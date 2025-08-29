#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <utility>
#include <fstream>
#include <filesystem>
#include <iostream>

void init();
std::string header;
std::string footer;
std::string style;
std::string page404;
std::string pageContact;
std::string pageSetup;
std::string pageClassInfo;
std::string pageHome;
std::array<std::string, 4> pageTemplate; // 0 title 1 style 2 contents 3

std::string genSiteMap(const std::string& input); // full sitemap document
std::string genClassInfo(const std::string& input); // full class info document
std::string genFunctionList(const std::string& input); // class name
std::string readWholeFile(const std::string& path); // file path
std::string genXML(const std::string& input); // full sitemap document
void getClassList();
std::string toLowerCase(const std::string& s);
std::vector<std::string> classes;
void fillPage(const std::string& path, const std::string& title, const std::string& contents);

int main(){
    init();
    getClassList();
    std::filesystem::create_directories("classinfo");
    std::filesystem::create_directories("functionlist");
    {
        std::ofstream f("sitemap.xml");
        f << genXML(readWholeFile("docs/sitemap.sg"));
    }
    fillPage("index.html", "SGEXTN Docs", pageHome + genSiteMap(readWholeFile("docs/sitemap.sg")));
    fillPage("404.html", "Error liao", page404);
    fillPage("contact.html", "Contact 05524F.sg", pageContact);
    fillPage("setup.html", "Set up SGEXTN", pageSetup);
    for(int i=0; i<classes.size(); i++){
        std::string& currentClass = classes.at(i);
        fillPage("classinfo/" + toLowerCase(currentClass) + ".html", currentClass + " Docs", genClassInfo(readWholeFile("docs/" + toLowerCase(currentClass) + ".sg")) + pageClassInfo);
        fillPage("functionlist/" + toLowerCase(currentClass) + ".html", currentClass + " Functions", genFunctionList(currentClass) + pageClassInfo);
    }
}

void fillPage(const std::string& path, const std::string& title, const std::string& contents){
    std::ofstream f(path);
    f << pageTemplate.at(0);
    f << title;
    f << pageTemplate.at(1);
    f << style;
    f << pageTemplate.at(2);
    f << header;
    f << contents;
    f << footer;
    f << pageTemplate.at(3);
}

void getClassList(){
    std::stringstream s(readWholeFile("docs/sitemap.sg"));
    std::string currentClass;
    while(s >> currentClass){
        if(currentClass.length() < 2 || currentClass.substr(0, 2) != "SG"){continue;}
        classes.push_back(currentClass);
    }
}

void init(){
    
    page404 = R"DELIM(
<h1 class="title" style="text-align: center;">Cannot find page leh</h1>
<p class="label" style="text-align: center;">Go check the link correct or not</p>
)DELIM";
    
    header = R"DELIM(
<div>
<button class="button" style="width: 100%;" onclick="location.href='/'">
<img src="https://raw.githubusercontent.com/InfinityIntegral/InfinityIntegral/main/flagemoji.png" style="vertical-align: middle; height: 1.5rem; image-rendering: auto;"> SGEXTN Documentation <img src="https://raw.githubusercontent.com/InfinityIntegral/InfinityIntegral/main/flagemoji.png" style="vertical-align: middle; height: 1.5rem; image-rendering: auto;">
</button>
</div>
)DELIM";
    
    footer = R"DELIM(
<div>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<p class="label">©2025 05524F.sg (Singapore)</p>
<p class="label"><a class="link" href="/contact">report a bug in SGEXTN or contact 05524F</a></p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
</div>
)DELIM";
    
    style = R"DELIM(
<link rel="icon" href="/icon.png" type="image/png">
<meta property="og:image" content="/icon.png">
<meta name="twitter:image" content="/icon.png">
<link rel="apple-touch-icon" href="/icon.png">

<script type="application/ld+json">
{
    "@context": "https://schema.org",
    "@type": "WebSite",
    "url": "https://infinityintegral.github.io",
    "image": "/icon.png"
}
</script>

<meta name="viewport" content="width=device-width, initial-scale=1.0">
<style>

@font-face {
	font-family: "SingScript.sg";
	src: url("/SingScript.sg")format("truetype");
	font-weight: normal;
	font-style: normal;
}

:root {
	--c0: rgb(0, 0, 0);
	--c1: rgb(64, 0, 50);
	--c2: rgb(128, 0, 100);
	--c3: rgb(191, 0, 150);
	--c4: rgb(255, 0, 200);
	--c5: rgb(255, 64, 214);
	--c6: rgb(255, 128, 227);
	--c7: rgb(255, 191, 241);
	--c8: rgb(255, 255, 255);
}

*{
        font-family: "SingScript.sg";
	max-width: 100%;
	word-break: break-word;
	-webkit-text-size-adjust: 100%;
	-ms-text-size-adjust: 100%;
}

.title {
	color: var(--c4);
	background-color: var(--c8);
	font-size: 3.5rem;
	margin: 0;
	font-weight: normal;
}
.title::selection {
	color: var(--c8);
	background-color: var(--c4);
	font-size: 3.5rem;
	margin: 0;
	font-weight: normal;
}

.halftitle {
	color: var(--c4);
	background-color: var(--c8);
	font-size: 2.5rem;
	margin: 0;
	font-weight: normal;
}
.halftitle::selection {
	color: var(--c8);
	background-color: var(--c4);
	font-size: 2.625rem;
	margin: 0;
	font-weight: normal;
}

.label {
	color: var(--c4);
	background-color: var(--c8);
	font-size: 1.75rem;
	margin: 0;
	white-space: pre-wrap;
}
.label::selection {
	color: var(--c8);
	background-color: var(--c4);
	font-size: 1.75rem;
	margin: 0;
	white-space: pre-wrap;
}

.link {
	color: var(--c2);
	background-color: var(--c8);
	text-decoration: underline;
}
.link:hover {
	color: var(--c2);
	background-color: var(--c7);
	text-decoration: underline;
}
.link:active {
	color: var(--c2);
	background-color: var(--c6);
	text-decoration: underline;
}
.link::selection {
	color: var(--c8);
	background-color: var(--c4);
	text-decoration: underline;
}

.button {
	color: var(--c8);
	background-color: var(--c4);
	font-size: 1.75rem;
	border: none;
}
.button:hover {
	color: var(--c7);
	background-color: var(--c3);
	font-size: 1.75rem;
	border: none;
}
.button:active {
	color: var(--c6);
	background-color: var(--c2);
	font-size: 1.75rem;
	border: none;
}
.button::selection {
	color: var(--c4);
	background-color: var(--c8);
	font-size: 1.75rem;
	border: none;
}

.input {
	color: var(--c8);
	background-color: var(--c4);
	font-size: 1.75rem;
	border: none;
	outline: none;
	text-align: center;
}
.input:hover {
	color: var(--c7);
	background-color: var(--c3);
	font-size: 1.75rem;
	border: none;
	outline: none;
	text-align: center;
}
.input:focus {
	color: var(--c6);
	background-color: var(--c2);
	font-size: 1.75rem;
	border: none;
	outline: none;
	text-align: center;
}
.input::selection {
	color: var(--c4);
	background-color: var(--c8);
	font-size: 1.75rem;
	border: none;
	outline: none;
	text-align: center;
}

</style>
)DELIM";
    
    pageContact = R"DELIM(
<h1 class="title">Contact 05524F</h1>
<p class="label">&#x9;You should contact 05524F immediately if anything in SGEXTN is not working as expected according to the documentation or if there are errors in the documentation. You can also suggest features to be added to SGEXTN, but 05524F is unlikely to accept your request unless it is really good. SGEXTN is licensed under LGPL, so you may change the internals or add new components if you want as long as you open source your changes or write to 05524F to apply for exemption from the license (more info below).<br>&#x9;You may contact 05524F by email or by Instagram. Note that your Instagram friend request will only be accepted if you know Student 05524F personally, otherwise, only email contact is allowed.</p>
<p class="label">email: <a class="link" href="mailto:infinityintegral333@gmail.com">infinityintegral333@gmail.com</a></p>
<p class="label">Instagram: <a class="link" href="https://www.instagram.com/infinityintegral.sg/?hl=en">@infinityintegral.sg</a><br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">Why 05524F</h1>
<p class="label">&#x9;The name of 05524F comes from Student 05524F, PSLE index number of the person writing code for 05524F. On official copyright statements such as those in licenses, the name 05524F.sg is written to credit Singapore.<br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">Legal info</h1>
<p class="label">&#x9;SGEXTN is licensed under <a class="link" href="https://opensource.org/license/lgpl-3-0">LGPL v3</a> (Lesser General Public License). This essentially means that you do not have to open source your work if using SGEXTN normally but should do so if you changed SGEXTN internals. Note that the SGUCentralManagement class is not considered SGEXTN internals and can be freely edited.<br>&#x9;Qt's license is completely separate from the SGEXTN license. You must follow their license strictly and 05524F cannot exempt you from the Qt license.<br>&#x9;If you changed SGEXTN but do not want to open source your work, you must get an exemption from 05524F first. 05524F is likely to grant license exemptions to residents and citizens of Singapore but the exemption is revoked automatically if you move to somewhere else or change your citizenship. 05524F can also grant license exemptions to projects that prominently feature the letters "SG" or "sg" in their names, but the exemption is revoked automatically if the project is renamed. In any case, 05524F must explicitly authorise exemptions and it only applies to the SGEXTN license, not the Qt license.<br>&nbsp;</p>
)DELIM";
    
    pageSetup = R"DELIM(
<h1 class="title">Setting up SGEXTN</h1>
<p class="label">&#x9;SGEXTN uses <a class="link" href="https://www.qt.io/">Qt</a> Quick extensively and can only be used in a Qt project. Qt version 6.9 and above is recommended.<br>&#x9;To use SGEXTN, copy the entire SGEXTN folder from the <a class="link" href="https://github.com/InfinityIntegral/SGEXTN">GitHub repository</a> into the root folder of your project which should contain your project file (.pro). To include it, write the line<br><br>&#x9;include(SGEXTN/SGEXTN.pri)<br><br> at the top of your project file (.pro). After that, go to the file SGEXTN/userDefinedClasses/sgucentralmanagement.cpp, read through the comments in the file and customise whatever is needed. Do not change anything in the SGEXTN folder unless specified by the file.<br>&#x9;Your custom code should be outside the SGEXTN folder. Classes in your custom code should have the prefix SGC. The first word in the names of renderer classes should be "Render".<br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">File prefixes</h1>
<p class="label">SGEXTN uses file prefixes to mean different things.<br>&#x9;SGX: SGEXTN classes not using SGWidget in any way<br>&#x9;SGW: SGEXTN classes using SGWidget and the SGWidget module itself<br>&#x9;SGU: customisable part of SGEXTN, can be changed by user<br>&#x9;SGC: user's custom code<br>Names of all renderer classes should start with the word "Render".<br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">Extending SGEXTN</h1>
<p class="label">&#x9;You can extend SGEXTN by adding renderers. Renderers can be used to display shader output, non rectangular UI, and large amounts of UI.<br>To build a renderer, you need the following:<br>&#x9;- a vertex shader (.vert) in GLSL, built using QSB into a Qt Shader Binary (.qsb) for deployment across all rendering backends<br>&#x9;- a fragment shader (.frag) in GLSL, built using QSB into a Qt Shader Binary (.qsb) for deployment across all rendering backends.<br>&#x9;- a QML file (.qml) describing the QML Component<br>&#x9;- C++ header (.h) and source (.cpp) files for the material<br>&#x9;- C++ header (.h) and source (.cpp) files for the shader, the shader must upload uniform data<br>&#x9;- C++ header (.h) and source (.cpp) files for the QQuickItem, this must set up the QQuickItem appropriately for rendering by Qt Quick<br>&#x9;- C++ header (.h) and source (.cpp) files for the SGWidget, this must inherit SGWCustomRenderer and set up the SGWidget appropriately<br>&#x9;- the type must be registered with the QML system, see comments on top of the dedicated function in SGEXTN/userDefinedClasses/sgucentralmanagement.cpp for more information<br><br>Refer to the SGEXTN/colourpickerrendering/huechoice folder for an example.<br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">UIB-DINB philosophy</h1>
<p class="label">&#x9;The SGEXTN framework is designed with the UIB-DINB philosophy in mind. UIB-DINB is "user is blur, developer is not blur", which is Singlish for "user gets confused, developer will not get confused".<br>&#x9;You, as a developer, are expected to be able to read SGEXTN documentation and source code if needed. You are expected to not be afraid of manual memory management, raw pointers, and function pointers. You should prioritise efficiency and user experience in your applications over time taken to build or ease of development. SGEXTN does not attempt to protect you from complexity excessively. For example, SGEXTN does not follow encapsulation and abstraction strictly inside the SGWidget module and does not follow these principles at all outside the module. You are discouraged from using shortcuts such as auto type because that may prevent you from fully understanding and optimising the code.<br>&#x9;You should design UI such that there is no way for the user to crash your app. If data entered is invalid, the user should be informed appropriately. Refer to the SingCorrect configuration page for what that looks like.<br>&nbsp;</p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<h1 class="title">Why SGEXTN</h1>
<p class="label">&#x9;SGEXTN allows you to use Qt Quick as a GUI toolkit without the annoying bits via SGWidget, such as QML declarative UI, JavaScript, signal-slot etc. This makes it much easier to use for someone familiar with C++ and does not know anything about JavaScript and related nonsense. Apart from SGWidget, there are also other things that most applications would need, such as file reading, file writing, and the use of colours.<br>&#x9;Using SGEXTN to build apps feel like QWidget but without the signal-slot callbacks. Also, make sure to become familiar with function pointer syntax first before using SGEXTN, because SGEXTN callbacks are raw function pointers.</p>
<h2 class="halftitle">QML?</h2>
<p class="label">&#x9;QML is the Qt Quick way of writing declarative UI using JSON syntax and with use of JavaScript for functions. Qt intends this to be similar to frontend development in website building. However, if one just want to write C++ and see widgets on screen, that would not work. Also, writing even tiny functions in JavaScript means dealing with all of JavaScript's nonsense, such as literally no types. By using SGEXTN's SGWidget system, you would not need a single line of QML when building UI.</p>
<h2 class="halftitle">signal-slot?</h2>
<p class="label">&#x9;Signal-slot is Qt's system for building callback functions. In Qt 6, they are essentially function pointers, which is good. However, using them requires the Q_OBJECT macro and MetaObject Compiler (MOC) processing, which increases compile time. SGEXTN abstracts that away by using raw function pointers for callbacks attached to timers (SGXTimer) and interactive UI elements including buttons (SGWButton subclasses) and input fields (SGWInput subclasses).</p>
<h2 class="halftitle">What if I want Qt stuff?</h2>
<p class="label">&#x9;SGEXTN abstracts Qt stuff without disabling them. So if you really want to use Qt stuff, you can do that normally.<br>&nbsp;</p>
)DELIM";
    
    pageClassInfo = R"DELIM(
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<p class="label">&#x9;Only properties, functions, and classes intended for public use have documentation. Your code autocompletion tool may provide suggestions that are not documented. You are discouraged from using these undocumented properties, functions, and classes unless you have read the source code and understand what they do.<br>&#x9;All information on this documentation page are only valid for SGEXTN version 5.1.0, future versions may have new features added or removed. For documentation of past versions, pls see the documentation bundled with the release.<br>&nbsp;</p>
)DELIM";
    
    pageHome = R"DELIM(
<h1 class="title">What is SGEXTN?</h1>
<p class="label">&#x9;SGEXTN is a framework built on top of <a class="link" href="https://www.qt.io/">Qt</a> Quick. It provides essential functionality for building apps, such as a colour struct and file system access. The SGWidget module provides a full GUI toolkit to build UI without writing a single line of QML. SGEXTN also does not use signal-slot, instead callback functions are implemented using function pointers.<br>&#x9;Note that documentation is only provided for the latest version. For earlier documentation, pls host the Documentation folder of your release yourself on <a class="link" href="https://www.apachefriends.org/">XAMPP Apache</a>.<br>Below is the full list of all SGEXTN classes with links to their documentation pages.</p>
<p class="label"><a class="link" href="/setup">how to set up SGEXTN</a></p>
<p class="label"><br></p>
<div style="width: 100%; height: 0.25em; background-color: var(--c4);"></div>
<p class="label">list of all classes that you can use inside SGEXTN 5.1.0:</p>
<p class="label" id="sitemap"></p>
)DELIM";
    
    pageTemplate = {"\n<!DOCTYPE html>\n<html lang=\"en-SG\">\n<head>\n<meta charset=\"UTF-8\">\n<title>", "</title>\n", "\n</head>\n<body>\n", "\n</body>\n</html>\n"};
    
}

std::string genSiteMap(const std::string& input){
    std::string output = "";
    int indent = 0;
    output += "<p class=\"label\">";
    std::stringstream stream(input);
    std::string s = "";
    while(std::getline(stream, s)){
        if(s == "<"){indent--;}
        else if(s == ">"){indent++;}
        else if(s.length() > 0 && s[0] == '/'){
            output += "\n";
            for(int i=0; i<indent; i++){output += "&#x9;";}
            output += s.substr(1);
        }
        else{
            output += "\n";
            for(int i=0; i<indent; i++){output += "&#x9;";}
            std::string sl = s;
            for(int i=0; i<sl.length(); i++){
                if(sl[i] >= 'A' && sl[i] <= 'Z'){sl[i] += ('a' - 'A');}
            }
            std::string link = "\"/classinfo/" + sl + "\"";
            output += ("<a class=\"link\" href=" + link + ">");
            output += s;
            output += "</a>";
        }
    }
    output += "\n</p>";
    return output;
}

std::string formatDescription(const std::string& s){
    std::string output = "&#x9;";
    for(int i=0; i<s.length(); i++){
        if(s[i] != '$'){output += s[i];}
        else{output += "<br>&#x9;";}
    }
    return output;
}

std::string toLowerCase(const std::string& s){
    std::string output = "";
    for(int i=0; i<s.length(); i++){
        if(s[i] >= 'a' && s[i] <= 'z'){output += s[i];}
        else if(s[i] >= 'A' && s[i] <= 'Z'){output += (s[i] - 'A' + 'a');}
        else if(s[i] >= '0' && s[i] <= '9'){output += s[i];}
    }
    return output;
}

void splitObj(const std::string& a, std::string& o1, std::string& o2, std::string& o3){
    int started = 0;
    o1 = "";
    o2 = "";
    o3 = "";
    for(int i=0; i<a.length(); i++){
        if(a[i] == '_' || (a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0' && a[i] <= '9')){
            if(started == 0){o1 += a[i];}
            else if(started == 1){o2 += a[i];}
            else if(started == 2){o3 += a[i];}
        }
        else if(a[i] == '$'){started = 1;}
        else{
            if(started == 0){o1 += a[i];}
            else if(started == 1){o3 += a[i]; started = 2;}
            else if(started == 2){o3 += a[i];}
        }
    }
}

std::string toHtmlTag(const std::string& s){
    std::string output = "#";
    bool x = false;
    for(int i=0; i<s.length(); i++){
        if(s[i] >= 'a' && s[i] <= 'z'){
            output += s[i];
            x = false;
        }
        else if(s[i] >= 'A' && s[i] <= 'Z'){
            output += (s[i] - 'A' + 'a');
            x = false;
        }
        else if(s[i] >= '0' && s[i] <= '9'){
            output += s[i];
            x = false;
        }
        else{
            if(x == false){
                output += '-';
                x = true;
            }
        }
    }
    return output;
}

void addObjList(const std::vector<std::pair<std::string, std::string>>& x, std::string& s){
    for(int i=0; i<x.size(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj(x.at(i).first, s1, s2, s3);
        s += "<p class=\"label\">&#x9;";
        s += s1;
        s += ("<a class=\"link\" href=\"" + toHtmlTag(x.at(i).first) + "\">" + s2 + "</a>");
        s += s3;
        s += "</p>\n";
    }
}

void addObjFull(const std::vector<std::pair<std::string, std::string>>& x, std::string& s){
    for(int i=0; i<x.size(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj(x.at(i).first, s1, s2, s3);
        s += ("<h2 class=\"halftitle\" id=\"" + toHtmlTag(x.at(i).first).substr(1) + "\">&#x9;");
        s += s1;
        s += ("<a class=\"link\" href=\"" + toHtmlTag(x.at(i).first) + "\">" + s2 + "</a>");
        s += s3;
        s += "</h2>\n";
        s += ("<p class=\"label\">" + formatDescription(x.at(i).second) + "<br>&nbsp;</p>\n");
    }
}

std::string genClassInfo(const std::string& input){
    std::string className;
    std::string classDescription;
    std::string include;
    std::string parentClass;
    std::vector<std::string> childrenClasses;
    std::string enumName;
    std::string enumDescription;
    std::vector<std::pair<std::string, std::string>> enumFlags;
    std::vector<std::pair<std::string, std::string>> memberProperties;
    std::vector<std::pair<std::string, std::string>> memberFunctions;
    std::vector<std::pair<std::string, std::string>> staticProperties;
    std::vector<std::pair<std::string, std::string>> staticFunctions;
    std::vector<std::pair<std::string, std::string>> memberReimplemented;
    std::vector<std::pair<std::string, std::string>> staticReimplemented;
    std::string laterDescription;
    std::vector<std::pair<std::string, std::string>> detailedDescription;
    
    std::stringstream stream(input);
    std::string s;
    while(std::getline(stream, s)){
        std::string type;
        std::string value;
        std::string sd;
        if(s.length() == 0){continue;}
        else if(s[0] != '#'){
            std::cout << ("invalid syntax at genClassInfo: " + s);
            return "";
        }
        for(int i=0; i<s.length(); i++){
            if(s[i] != ' '){type += s[i];}
            else{
                value = s.substr(i+1);
                break;
            }
        }
        type = type.substr(1);
        if(type == "c"){
            className = value;
            std::getline(stream, sd);
            classDescription = sd;
        }
        else if(type == "ic"){include = value;}
        else if(type == "if"){parentClass = value;}
        else if(type == "it"){childrenClasses.push_back(value);}
        else if(type == "e"){
            enumName = value;
            std::getline(stream, enumDescription);
        }
        else if(type == "ef"){
            std::getline(stream, sd);
            enumFlags.push_back(std::make_pair(value, sd));
        }
        else if(type == "p"){
            std::getline(stream, sd);
            memberProperties.push_back(std::make_pair(value, sd));
        }
        else if(type == "f"){
            std::getline(stream, sd);
            memberFunctions.push_back(std::make_pair(value, sd));
        }
        else if(type == "sp"){
            std::getline(stream, sd);
            staticProperties.push_back(std::make_pair(value, sd));
        }
        else if(type == "sf"){
            std::getline(stream, sd);
            staticFunctions.push_back(std::make_pair(value, sd));
        }
        else if(type == "r"){
            std::getline(stream, sd);
            memberReimplemented.push_back(std::make_pair(value, sd));
        }
        else if(type == "sr"){
            std::getline(stream, sd);
            staticReimplemented.push_back(std::make_pair(value, sd));
        }
        else if(type == "d"){laterDescription = value;}
        else if(type == "dt"){
            std::getline(stream, sd);
            detailedDescription.push_back(std::make_pair(value, sd));
        }
        else{
            std::cout << ("invalid type at genClassInfo: " + type);
            return "";
        }
    }
    
    std::string output = "";
    output += ("<h1 class=\"title\">" + className + "</h1>\n");
    output += ("<p class=\"label\">" + formatDescription(classDescription) + "</p>\n");
    output += ("<p class=\"label\">&nbsp;<br>&#x9;header: #include\" " + include + "\"</p>\n");
    if(parentClass == ""){output += "<p class=\"label\">&#x9;inherits: none</p>\n";}
    else{output += ("<p class=\"label\">&#x9;inherits: <a class=\"link\" href=\"/classinfo/" + toLowerCase(parentClass) + "\">" + parentClass + "</a></p>\n");}
    if(childrenClasses.size() > 0){
        output += "<p class=\"label\">&#x9;inherited by:</p>\n";
        for(int i=0; i<childrenClasses.size(); i++){
            output += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"/classinfo/" + toLowerCase(childrenClasses[i]) + "\">" + childrenClasses[i] + "</a></p>\n");
        }
    }
    output += "<p class=\"label\">&#x9;<a class=\"link\" href=\"#moreinfo\">More information...</a></p>";
    output += "<p class=\"label\">&#x9;<a class=\"link\" href=\"/functionlist/" + toLowerCase(className) + "\">List of all including inherited members</a></p>";
    output += "<p class=\"label\">&nbsp;</p>";
    
    if(enumName != ""){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Attached Enum</h2>\n";
        output += ("<p class=\"label\">&#x9;enum <a class=\"link\" href=\"#enum" + toLowerCase(enumName) + "\">" +  enumName + "</a></p>\n");
        for(int i=0; i<enumFlags.size(); i++){
            output += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"#flag" + toLowerCase(enumFlags.at(i).first) + "\">" + enumFlags.at(i).first + "</a></p>\n");
        }
        output += "<p class=\"label\">&#x9;</p>";
    }
    
    if(memberProperties.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Instance Properties</h2>\n";
        addObjList(memberProperties, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    if(memberFunctions.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Instance Methods</h2>\n";
        addObjList(memberFunctions, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    if(memberReimplemented.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Reimplemented Instance Methods</h2>\n";
        addObjList(memberReimplemented, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    if(staticProperties.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Static Properties</h2>\n";
        addObjList(staticProperties, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    if(staticFunctions.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Static Methods</h2>\n";
        addObjList(staticFunctions, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    if(staticReimplemented.size() > 0){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Reimplemented Static Methods</h2>\n";
        addObjList(staticReimplemented, output);
        output += "<p class=\"label\">&nbsp;</p>";
    }
    
    output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
    output += "<h2 class=\"halftitle\" id=\"moreinfo\">Detailed Description</h2>\n";
    output += ("<p class=\"label\">" + formatDescription(laterDescription) + "</p>\n");
    for(int i=0; i<detailedDescription.size(); i++){
        output += ("<h2 class=\"halftitle\">" + detailedDescription.at(i).first + "</h2>\n");
        output += ("<p class=\"label\">" + formatDescription(detailedDescription.at(i).second) + "</p>\n");
    }
    output += "<p class=\"label\">&nbsp;</p>";
    output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
    
    if(enumName != ""){
        output += ("<h2 class=\"halftitle\" id=\"enum" + toLowerCase(enumName) + "\"><a class=\"link\" href=\"#enum" + toLowerCase(enumName) + "\">" + enumName + "</a></h2>\n");
        output += ("<p class=\"label\">" + formatDescription(enumDescription) + "<br>&nbsp;</p>\n");
        for(int i=0; i<enumFlags.size(); i++){
            output += ("<h2 class=\"halftitle\" id=\"flag" + toLowerCase(enumFlags.at(i).first) + "\">" + enumName + " <a class=\"link\" href=\"#flag" + toLowerCase(enumFlags.at(i).first) + "\">" + enumFlags.at(i).first + "</a></h2>\n");
            output += ("<p class=\"label\">" + formatDescription(enumFlags.at(i).second) + "<br>&nbsp;</p>\n");
        }
    }
    
    addObjFull(memberProperties, output);
    addObjFull(memberFunctions, output);
    addObjFull(memberReimplemented, output);
    addObjFull(staticProperties, output);
    addObjFull(staticFunctions, output);
    addObjFull(staticReimplemented, output);
    return output;
}

struct ClassInfo{
    ClassInfo(){
        className = "";
        enumName = "";
        enumFlags = std::vector<std::string>();
        memberProperties = std::vector<std::string>();
        memberFunctions = std::vector<std::string>();
        staticProperties = std::vector<std::string>();
        staticFunctions = std::vector<std::string>();
        memberReimplemented = std::vector<std::string>();
        staticReimplemented = std::vector<std::string>();
    }
    std::string className;
    std::string parentName;
    std::string enumName;
    std::vector<std::string> enumFlags;
    std::vector<std::string> memberProperties;
    std::vector<std::string> memberFunctions;
    std::vector<std::string> staticProperties;
    std::vector<std::string> staticFunctions;
    std::vector<std::string> memberReimplemented;
    std::vector<std::string> staticReimplemented;
};

ClassInfo getClassInfo(const std::string& input){
    ClassInfo output;
    std::stringstream stream(input);
    std::string s;
    while(std::getline(stream, s)){
        std::string type;
        std::string value;
        std::string sd;
        if(s.length() == 0){continue;}
        else if(s[0] != '#'){
            std::cout << ("invalid syntax at getClassInfo: " + s);
            return ClassInfo();
        }
        for(int i=0; i<s.length(); i++){
            if(s[i] != ' '){type += s[i];}
            else{
                value = s.substr(i+1);
                break;
            }
        }
        type = type.substr(1);
        if(type == "c"){
            output.className = value;
            std::getline(stream, sd);
        }
        else if(type == "if"){
            output.parentName = value;
        }
        else if(type == "e"){
            output.enumName = value;
            std::getline(stream, sd);
        }
        else if(type == "ef"){
            std::getline(stream, sd);
            output.enumFlags.push_back(value);
        }
        else if(type == "p"){
            std::getline(stream, sd);
            output.memberProperties.push_back(value);
        }
        else if(type == "f"){
            std::getline(stream, sd);
            output.memberFunctions.push_back(value);
        }
        else if(type == "sp"){
            std::getline(stream, sd);
            output.staticProperties.push_back(value);
        }
        else if(type == "sf"){
            std::getline(stream, sd);
            output.staticFunctions.push_back(value);
        }
        else if(type == "r"){
            std::getline(stream, sd);
            output.memberReimplemented.push_back(value);
        }
        else if(type == "sr"){
            std::getline(stream, sd);
            output.staticReimplemented.push_back(value);
        }
        else if(type == "dt"){std::getline(stream, sd);}
    }
    return output;
}

std::string readWholeFile(const std::string& path){
    std::ifstream f(path, std::ios::binary);
    std::ostringstream s;
    s << f.rdbuf();
    return s.str();
}

std::vector<ClassInfo> getClassInfoList(const std::string& input){
    std::vector<ClassInfo> reversedList;
    std::string currentClass = input;
    while(currentClass != ""){
        ClassInfo nextClass = getClassInfo(readWholeFile("docs/" + toLowerCase(currentClass) + ".sg"));
        reversedList.push_back(nextClass);
        currentClass = nextClass.parentName;
    }
    std::vector<ClassInfo> output;
    for(int i=reversedList.size()-1; i>=0; i--){
        output.push_back(reversedList.at(i));
    }
    return output;
}

void addObjList0(const std::vector<std::string>& x, std::string& s, std::string cn){
    for(int i=0; i<x.size(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj(x.at(i), s1, s2, s3);
        s += "<p class=\"label\">&#x9;";
        s += s1;
        s += ("<a class=\"link\" href=\"/classinfo/" + toLowerCase(cn) + toHtmlTag(x.at(i)) + "\">" + cn + "::" + s2 + "</a>");
        s += s3;
        s += "</p>\n";
    }
}

std::string genFunctionList(const std::string& input){
    std::vector<ClassInfo> data = getClassInfoList(input);
    std::string enumHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Attached Enum</h2>\n";
    bool enumUsed = false;
    std::string pHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Instance Properties</h2>\n";
    bool pUsed = false;
    std::string fHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Instance Methods</h2>\n";
    bool fUsed = false;
    std::string rHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Reimplemented Instance Methods</h2>\n";
    bool rUsed = false;
    std::string spHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Static Properties</h2>\n";
    bool spUsed = false;
    std::string sfHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Static Methods</h2>\n";
    bool sfUsed = false;
    std::string srHTML = "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n<h2 class=\"halftitle\">Reimplemented Static Methods</h2>\n";
    bool srUsed = false;
    
    for(int i=0; i<data.size(); i++){
        ClassInfo& currentClass = data.at(i);
        
        if(currentClass.enumName != ""){
            enumUsed = true;
            enumHTML += ("<p class=\"label\">&#x9;enum <a class=\"link\" href=\"/classinfo/" + toLowerCase(currentClass.className) + "#enum" + toLowerCase(currentClass.enumName) + "\">" + currentClass.className + "::" +  currentClass.enumName + "</a></p>\n");
            for(int i=0; i<currentClass.enumFlags.size(); i++){
                enumHTML += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"/classinfo/" + toLowerCase(currentClass.className) + "#flag" + toLowerCase(currentClass.enumFlags.at(i)) + "\">" + currentClass.className + "::" + currentClass.enumFlags.at(i) + "</a></p>\n");
            }
        }
        
        if(currentClass.memberProperties.size() > 0){
            pUsed = true;
            addObjList0(currentClass.memberProperties, pHTML, currentClass.className);
        }
        if(currentClass.memberFunctions.size() > 0){
            fUsed = true;
            addObjList0(currentClass.memberFunctions, fHTML, currentClass.className);
        }
        if(currentClass.memberReimplemented.size() > 0){
            rUsed = true;
            addObjList0(currentClass.memberReimplemented, rHTML, currentClass.className);
        }
        if(currentClass.staticProperties.size() > 0){
            spUsed = true;
            addObjList0(currentClass.staticProperties, spHTML, currentClass.className);
        }
        if(currentClass.staticFunctions.size() > 0){
            sfUsed = true;
            addObjList0(currentClass.staticFunctions, sfHTML, currentClass.className);
        }
        if(currentClass.staticReimplemented.size() > 0){
            srUsed = true;
            addObjList0(currentClass.staticReimplemented, srHTML, currentClass.className);
        }
    }
    
    std::string output = "";
    output += "<h1 class=\"title\">" + data.at(data.size() - 1).className + "</h1>\n";
    output += "<p class=\"label\">&#x9;This page contains a list of all properties and methods found in " + data.at(data.size() - 1).className + ", including inherited ones. By clicking the link on each property or method, you can access its documentation page.</p>\n";
    output += "<p class=\"label\">&nbsp;</p>";
    if(enumUsed == true){
        output += enumHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(pUsed == true){
        output += pHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(fUsed == true){
        output += fHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(rUsed == true){
        output += rHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(spUsed == true){
        output += spHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(sfUsed == true){
        output += sfHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    if(srUsed == true){
        output += srHTML;
        output += "<p class=\"label\">&nbsp;</p>";
    }
    return output;
}

std::string genXML(const std::string& input){
    std::stringstream s(input);
    std::string output;
    output += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output += "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
    output += "<url><loc>https://infinityintegral.github.io/</loc><priority>1.0</priority></url>\n";
    output += "<url><loc>https://infinityintegral.github.io/setup</loc><priority>1.0</priority></url>\n";
    output += "<url><loc>https://infinityintegral.github.io/contact</loc><priority>0.3</priority></url>\n";
    std::string currentClass;
    while(s >> currentClass){
        if(currentClass.length() < 2 || currentClass.substr(0, 2) != "SG"){continue;}
        currentClass = toLowerCase(currentClass);
        output += ("<url><loc>https://infinityintegral.github.io/classinfo/" + currentClass + "</loc><priority>0.8</priority></url>\n");
        output += ("<url><loc>https://infinityintegral.github.io/functionlist/" + currentClass + "</loc><priority>0.5</priority></url>\n");
    }
    output += "</urlset>";
    return output;
}
