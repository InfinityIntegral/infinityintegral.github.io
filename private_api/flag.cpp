#include <vector>
#include <cmath>
#include <GLES2/gl2.h>
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>
#include <algorithm>
#include <random>

// emcc private_api/flag.cpp -o index.html --shell-file=private_api/template.html -s USE_WEBGL2=0 -s FULL_ES2=1 -s ALLOW_MEMORY_GROWTH=1 -std=c++20 -v

class Vector3 {
public:
    Vector3() = default;
    Vector3(float x, float y, float z);
    float x;
    float y;
    float z;
    Vector3 operator+(Vector3 x);
    Vector3 operator-(Vector3 x);
    Vector3 operator*(float x);
    Vector3 operator/(float x);
    Vector3& operator+=(Vector3 x);
    Vector3& operator-=(Vector3 x);
    Vector3& operator*=(float x);
    Vector3& operator/=(float x);
    float magnitude();
};

class Particle {
public:
    Particle() = default;
    Particle(int x, int y);
    int x;
    int y;
    Vector3 position;
    Vector3 lastPosition;
    Vector3 normal;
};

class Flag {
public:
    Flag();
    std::vector<std::vector<Particle>> particles;
    Particle& at(int x, int y);
    static float timeStep;
    static int width;
    static int height;
    static Flag instance;
    static int iterationCount;
    static float unit;
    void updateFrame();
    static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE renderingContext;
    static void initialiseRendering();
    static GLuint shaderProgramme;
    static GLuint vbo;
    static GLuint ebo;
    static void advanceSimulation();
    static int frameCount;
};

class PerlinNoise {
public:
    static std::vector<int> xTable;
    static std::vector<int> yTable;
    static std::vector<int> zTable;
    static std::vector<int> generateLookupTable();
    static float fade(float x);
    static float interpolate(float a, float b, float x);
    static float gradient(int hash, float x, float y, float z);
    static float generate(float x, float y, float z, const std::vector<int>& seedTable);
};

Vector3::Vector3(float x, float y, float z){
    (*this).x = x;
    (*this).y = y;
    (*this).z = z;
}

Vector3 Vector3::operator+(Vector3 x){
    return Vector3((*this).x + x.x, (*this).y + x.y, (*this).z + x.z);
}

Vector3 Vector3::operator-(Vector3 x){
    return Vector3((*this).x - x.x, (*this).y - x.y, (*this).z - x.z);
}

Vector3 Vector3::operator*(float x){
    return Vector3((*this).x * x, (*this).y * x, (*this).z * x);
}

Vector3 Vector3::operator/(float x){
    return Vector3((*this).x / x, (*this).y / x, (*this).z / x);
}

Vector3& Vector3::operator+=(Vector3 x){
    (*this).x += x.x;
    (*this).y += x.y;
    (*this).z += x.z;
    return (*this);
}

Vector3& Vector3::operator-=(Vector3 x){
    (*this).x -= x.x;
    (*this).y -= x.y;
    (*this).z -= x.z;
    return (*this);
}

Vector3& Vector3::operator*=(float x){
    (*this).x *= x;
    (*this).y *= x;
    (*this).z *= x;
    return (*this);
}

Vector3& Vector3::operator/=(float x){
    (*this).x /= x;
    (*this).y /= x;
    (*this).z /= x;
    return (*this);
}

float Vector3::magnitude(){
    return std::sqrtf(x*x + y*y + z*z);
}

Particle::Particle(int x, int y){
    (*this).x = x;
    (*this).y = y;
    (*this).position = Vector3(0.0f, Flag::unit * static_cast<float>(y), 0.0f);
    (*this).lastPosition = (*this).position;
}

float Flag::timeStep = 1.0f / 60.0f;
int Flag::width = 30;
int Flag::height = 30;
Flag Flag::instance = Flag();
int Flag::iterationCount = 10;
float Flag::unit = 0.1f;
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE Flag::renderingContext;
GLuint Flag::shaderProgramme = 0;
GLuint Flag::vbo = 0;
GLuint Flag::ebo = 0;
int Flag::frameCount = 0;

Flag::Flag(){
    for(int j=0; j<Flag::height; j++){
        particles.push_back(std::vector<Particle>());
        for(int i=0; i<Flag::width; i++){
            particles.at(j).push_back(Particle(i, j));
        }
    }
}

Particle& Flag::at(int x, int y){
    if(x < 0){x = 0;}
    else if(x >= Flag::width){x = Flag::width - 1;}
    if(y < 0){y = 0;}
    else if(y >= Flag::height){y = Flag::height - 1;}
    return particles.at(y).at(x);
}

void Flag::initialiseRendering(){
    EmscriptenWebGLContextAttributes contextAttributes;
    emscripten_webgl_init_context_attributes(&contextAttributes);
    contextAttributes.alpha = true;
    contextAttributes.depth = true;
    contextAttributes.stencil = false;
    contextAttributes.antialias = true;
    contextAttributes.majorVersion = 1;
    Flag::renderingContext = emscripten_webgl_create_context("#canvas", &contextAttributes);
    emscripten_webgl_make_context_current(Flag::renderingContext);
    const char* vertexShaderSourceCode = R"(
precision highp float;
attribute vec3 vertex;
attribute vec3 vertexNormal;
attribute vec2 vertexTexCoord;
varying vec3 normal;
varying vec2 texCoord;
void main(){
    gl_Position = vec4(vertex.x / 3.0 - 0.5, -1.0 * (vertex.y / 3.0 - 0.5), vertex.z, 1.0);
    normal = vertexNormal;
    texCoord = vec2(216.0 * vertexTexCoord.x, 144.0 * vertexTexCoord.y);
}
)";
    const char* fragmentShaderSourceCode = R"(
precision highp float;
varying vec3 normal;
varying vec2 texCoord;

float atan2(float y, float x){
    float pi = 3.1415926535897932384626433832795028841971;
    if(x > 0.0){return atan(y / x);}
    if(x < 0.0){
        float a = atan(y / x);
        if(y >= 0.0){return a + pi;}
        return a - pi;
    }
    if(y > 0.0){return 0.5 * pi;}
    if(y < 0.0){return -0.5 * pi;}
    return 0.0;
}

float adjustedAtan2(float y, float x){
    float pi = 3.1415926535897932384626433832795028841971;
    return mod(atan2(y, x) * 180.0 / pi + 360.0 + 90.0, 360.0);
}

bool isInCircle(vec2 point, vec2 center, float radius){
    return ((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y) < radius * radius);
}

bool isInStar(vec2 point, vec2 center, float radius){
    float pi = 3.1415926535897932384626433832795028841971;
    float dist = sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y)) / radius;
    float a = adjustedAtan2(point.y - center.y, point.x - center.x);
    a = abs(mod(a, 72.0) - 36.0);
    a = sin(0.1 * pi) / sin(0.1 * pi + (36.0 - a) * pi / 180.0);
    return (dist <= a);
}

void main(){
    vec4 red = vec4(238.0 / 255.0, 37.0 / 255.0, 54.0 / 255.0, 1.0);
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 colour = vec4(0.0, 0.0, 0.0, 0.0);
    if(texCoord.y > 72.0){colour = white;}
    else if(texCoord.x > 108.0){colour = red;}
    else if(isInCircle(texCoord, vec2(45.5, 36.0), 26.5) == true && isInCircle(texCoord, vec2(60.0, 36.0), 29.0) == false){colour = white;}
    else if(isInStar(texCoord, vec2(60.0, 20.8), 6.65) == true){colour = white;}
    else if(isInStar(texCoord, vec2(74.45606, 31.30294), 6.65) == true){colour = white;}
    else if(isInStar(texCoord, vec2(45.54394, 31.30294), 6.65) == true){colour = white;}
    else if(isInStar(texCoord, vec2(68.93434, 48.29706), 6.65) == true){colour = white;}
    else if(isInStar(texCoord, vec2(51.06566, 48.29706), 6.65) == true){colour = white;}
    else{colour = red;}
    float m = 0.95 * max(dot(normal, vec3(0.0, 0.0, 1.0)), 0.0);
    gl_FragColor = vec4(colour.r * m, colour.g * m, colour.b * m, 1.0);
}
)";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCode, nullptr);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, nullptr);
    glCompileShader(fragmentShader);
    GLuint shaderProgramme = glCreateProgram();
    glAttachShader(shaderProgramme, vertexShader);
    glAttachShader(shaderProgramme, fragmentShader);
    glLinkProgram(shaderProgramme);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    Flag::shaderProgramme = shaderProgramme;
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, Flag::width * Flag::height * (3 * 4 + 3 * 4 + 2 * 4), nullptr, GL_DYNAMIC_DRAW);
    std::vector<GLuint> eboData;
    eboData.reserve((Flag::width - 1) * (Flag::height - 1) * 2 * 3);
    for(int j=0; j<Flag::height-1; j++){
        for(int i=0; i<Flag::width-1; i++){
            eboData.push_back(j * Flag::width + i);
            eboData.push_back(j * Flag::width + i + 1);
            eboData.push_back((j + 1) * Flag::width + i);
            eboData.push_back(j * Flag::width + i + 1);
            eboData.push_back((j + 1) * Flag::width + i);
            eboData.push_back((j + 1) * Flag::width + i + 1);
        }
    }
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboData.size() * sizeof(GLuint), eboData.data(), GL_STATIC_DRAW);
    Flag::vbo = vbo;
    Flag::ebo = ebo;
    glUseProgram(Flag::shaderProgramme);
    glBindBuffer(GL_ARRAY_BUFFER, Flag::vbo);
    GLuint location = glGetAttribLocation(Flag::shaderProgramme, "vertex");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * 4 + 3 * 4 + 2 * 4, (void*)0);
    location = glGetAttribLocation(Flag::shaderProgramme, "vertexNormal");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * 4 + 3 * 4 + 2 * 4, (void*)(3 * 4));
    location = glGetAttribLocation(Flag::shaderProgramme, "vertexTexCoord");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 3 * 4 + 3 * 4 + 2 * 4, (void*)(3 * 4 + 3 * 4));
    PerlinNoise::xTable = PerlinNoise::generateLookupTable();
    PerlinNoise::yTable = PerlinNoise::generateLookupTable();
    PerlinNoise::zTable = PerlinNoise::generateLookupTable();
}

void Flag::updateFrame(){
    Flag::frameCount++;
    for(int i=1; i<Flag::width; i++){
        for(int j=0; j<Flag::height; j++){
            float fx = static_cast<float>(i) / 20.0f;
            float fy = static_cast<float>(j) / 20.0f;
            float fz = static_cast<float>(Flag::frameCount) / 50.0f;
            Vector3 acceleration = Vector3(5.0f, 0.0f, 0.0f) + Vector3(PerlinNoise::generate(fx, fy, fz, PerlinNoise::xTable), PerlinNoise::generate(fx, fy, fz, PerlinNoise::yTable), PerlinNoise::generate(fx, fy, fz, PerlinNoise::zTable)) * 0.5f;
            Vector3 movement = (at(i, j).position - at(i, j).lastPosition) + acceleration * Flag::timeStep * Flag::timeStep;
            at(i, j).lastPosition = at(i, j).position;
            at(i, j).position += movement;
        }
    }
    for(int repeat=0; repeat<Flag::iterationCount; repeat++){
        for(int i=1; i<Flag::width; i++){
            for(int j=0; j<Flag::height; j++){
                if(i == 1){
                    Vector3 diff = at(i, j).position - at(i - 1, j).position;
                    at(i, j).position = at(i - 1, j).position + diff * Flag::unit / diff.magnitude();
                }
                else{
                    Vector3 diff = at(i, j).position - at(i - 1, j).position;
                    Vector3 average = (at(i, j).position + at(i - 1, j).position) * 0.5f;
                    at(i, j).position = average + diff * 0.5f * Flag::unit / diff.magnitude();
                    at(i - 1, j).position = average - diff * 0.5f * Flag::unit / diff.magnitude();
                }
            }
        }
        for(int i=1; i<Flag::width; i++){
            for(int j=1; j<Flag::height; j++){
                Vector3 diff = at(i, j).position - at(i, j-1).position;
                Vector3 average = (at(i, j).position + at(i, j-1).position) * 0.5f;
                at(i, j).position = average + diff * 0.5f * Flag::unit / diff.magnitude();
                at(i, j-1).position = average - diff * 0.5f * Flag::unit / diff.magnitude();
            }
        }
    }
    for(int i=0; i<Flag::width; i++){
        for(int j=0; j<Flag::height; j++){
            Vector3 dx = at(i + 1, j).position - at(i - 1, j).position;
            Vector3 dy = at(i, j - 1).position - at(i, j + 1).position;
            Vector3 normal = Vector3(dy.y * dx.z - dy.z * dx.y, dy.z * dx.x - dy.x * dx.z, dy.x * dx.y - dy.y * dx.x);
            if(normal.magnitude() > 0.00001f){normal /= normal.magnitude();}
            else{normal = Vector3(0.0f, 0.0f, 1.0f);}
            if(normal.z < 0.0f){normal = normal * (-1.0f);}
            at(i, j).normal = normal;
        }
    }
    float w = EM_ASM_INT({return document.documentElement.clientWidth;});
    glViewport(0, 0, static_cast<int>(w), static_cast<int>(2.0f / 3.0f * w));
    std::vector<GLfloat> vboData;
    vboData.reserve(3 * Flag::width * Flag::height);
    for(int j=0; j<Flag::height; j++){
        for(int i=0; i<Flag::width; i++){
            vboData.push_back(at(i, j).position.x);
            vboData.push_back(at(i, j).position.y);
            vboData.push_back(at(i, j).position.z);
            vboData.push_back(at(i, j).normal.x);
            vboData.push_back(at(i, j).normal.y);
            vboData.push_back(at(i, j).normal.z);
            vboData.push_back(static_cast<float>(i) / static_cast<float>(Flag::width - 1));
            vboData.push_back(static_cast<float>(j) / static_cast<float>(Flag::height - 1));
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, Flag::vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, Flag::width * Flag::height * (3 * 4 + 3 * 4 + 2 * 4), vboData.data());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(Flag::shaderProgramme);
    glBindBuffer(GL_ARRAY_BUFFER, Flag::vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Flag::ebo);
    glDrawElements(GL_TRIANGLES, (Flag::width - 1) * (Flag::height - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
}

void Flag::advanceSimulation(){
    float w = EM_ASM_INT({return document.documentElement.clientWidth;});
    emscripten_set_canvas_element_size("#canvas", static_cast<int>(w), static_cast<int>(2.0f / 3.0f * w));
    Flag::instance.updateFrame();
}

std::vector<int> PerlinNoise::xTable;
std::vector<int> PerlinNoise::yTable;
std::vector<int> PerlinNoise::zTable;

std::vector<int> PerlinNoise::generateLookupTable(){
    std::vector<int> table;
    table.reserve(512);
    for(int i=0; i<256; i++){
        table.push_back(i);
    }
    std::random_device rngDevice;
    std::mt19937 rng(rngDevice());
    std::shuffle(table.data(), table.data() + table.size(), rng);
    for(int i=0; i<256; i++){
        table.push_back(table.at(i));
    }
    return table;
}

float PerlinNoise::fade(float x){
    return (x * x * x * (x * (x * 6 - 15) + 10));
}

float PerlinNoise::interpolate(float a, float b, float x){
    return (a + x * (b - a));
}

float PerlinNoise::gradient(int hash, float x, float y, float z){
    int h = hash & 15;
    float u;
    float v;
    if(h < 8){u = x;}
    else{u = y;}
    if(h < 4){v = y;}
    else if(h == 12 || h == 14){v = x;}
    else{v = z;}
    float result = 0.0f;
    if((h & 1) != 0){result -= u;}
    else{result += u;}
    if((h & 2) != 0){result -= v;}
    else{result += v;}
    return result;
}

float PerlinNoise::generate(float x, float y, float z, const std::vector<int> &seedTable){
    int zx = static_cast<int>(std::floorf(x)) & 255;
    int zy = static_cast<int>(std::floorf(y)) & 255;
    int zz = static_cast<int>(std::floorf(z)) & 255;
    x -= std::floorf(x);
    y -= std::floorf(y);
    z -= std::floorf(z);
    float u = PerlinNoise::fade(x);
    float v = PerlinNoise::fade(y);
    float w = PerlinNoise::fade(z);
    int a = seedTable.at(zx) + zy;
    int aa = seedTable.at(a) + zz;
    int ab = seedTable.at(a + 1) + zz;
    int b = seedTable.at(zx + 1) + zy;
    int ba = seedTable.at(b) + zz;
    int bb = seedTable.at(b + 1) + zz;
    float g000 = PerlinNoise::gradient(seedTable.at(aa), x, y, z);
    float g100 = PerlinNoise::gradient(seedTable.at(ba), x - 1, y, z);
    float g010 = PerlinNoise::gradient(seedTable.at(ab), x, y - 1, z);
    float g110 = PerlinNoise::gradient(seedTable.at(bb), x - 1, y - 1, z);
    float g001 = PerlinNoise::gradient(seedTable.at(aa + 1), x, y, z - 1);
    float g101 = PerlinNoise::gradient(seedTable.at(ba + 1), x - 1, y, z - 1);
    float g011 = PerlinNoise::gradient(seedTable.at(ab + 1), x, y - 1, z - 1);
    float g111 = PerlinNoise::gradient(seedTable.at(bb + 1), x - 1, y - 1, z - 1);
    float g00 = PerlinNoise::interpolate(g000, g100, u);
    float g10 = PerlinNoise::interpolate(g010, g110, u);
    float g01 = PerlinNoise::interpolate(g001, g101, u);
    float g11 = PerlinNoise::interpolate(g011, g111, u);
    float g0 = PerlinNoise::interpolate(g00, g10, v);
    float g1 = PerlinNoise::interpolate(g01, g11, v);
    return PerlinNoise::interpolate(g0, g1, w);
}

int main(){
    Flag::initialiseRendering();
    emscripten_set_main_loop(&Flag::advanceSimulation, 0, 1);
}
