#include <fstream>
#include <string>

std::string toLowerCase(std::string s){
    std::string o;
    for(int i=0; i<s.length(); i++){
        if(s[i] == '_' || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9')){o += s[i];}
        else if(s[i] >= 'A' && s[i] <= 'Z'){o += (s[i] + 'a' - 'A');}
    }
    return o;
}

int main(){
    std::ifstream inFile("classlist.txt");
    std::ofstream outFile("sitemap.xml");
    std::string output;
    output += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output += "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
    output += "<url><loc>https://infinityintegral.github.io/</loc><priority>1.0</priority></url>\n";
    output += "<url><loc>https://infinityintegral.github.io/setup.html</loc><priority>1.0</priority></url>\n";
    output += "<url><loc>https://infinityintegral.github.io/contact.html</loc><priority>0.3</priority></url>\n";
    std::string currentClass;
    while(inFile >> currentClass){
        currentClass = toLowerCase(currentClass);
        output += ("<url><loc>https://infinityintegral.github.io/classinfo.html?classname=" + currentClass + "</loc><priority>0.8</priority></url>\n");
        output += ("<url><loc>https://infinityintegral.github.io/functionlist.html?classname=" + currentClass + "</loc><priority>0.5</priority></url>\n");
    }
    output += "</urlset>";
    outFile << output;
}
