#include <string>
#include <sstream>
#include <vector>
#include <pair>
#include <map>

std::string formatDescription(std::string s){
    std::string output = "&#x9;";
    for(int i=0; i<s.length(); i++){
        if(s[i] != '$'){output += s[i];}
        else{output += "<br>&#x9;";}
    }
    return output;
}

std::string toLowerCase(std::string s){
    std::string output = "";
    for(int i=0; i<s.length(); i++){
        if(s[i] >= 'a' && s[i] <= 'z'){output += s[i];}
        else if(s[i] >= 'A' && s[i] <= 'Z'){output += (s[i] - 'A' + 'a');}
        else if(s[i] >= '0' && s[i] <= '9'){output += s[i];}
    }
    return output;
}

bool compareObj(std::string a, std::string b){
    std::string sa = "";
    std::string sb = "";
    bool started = false;
    for(int i=0; i<a.length(); i++){
        if(a[i] == "_" || (a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0' && a[i] <= '9')){
            if(started == true){sa += a[i];}
        }
        else if(a[i] == '$'){started = true;}
        else{break;}
    }
    started = false;
    for(int i=0; i<b.length(); i++){
        if(b[i] == "_" || (b[i] >= 'a' && b[i] <= 'z') || (b[i] >= 'A' && b[i] <= 'Z') || (a[i] >= '0' && a[i] <= '9')){
            if(started == true){sb += b[i];}
        }
        else if(b[i] == '$'){started = true;}
        else{break;}
    }
    if(started == false){return (a < b);}
    return sa < sb;
}

struct StringComparator{
    bool operator()(const std::string& a, const std::string& b) const {
        return compareObj(a, b);
    }
};

void splitObj(std::string a, std::string& o1, std::string& o2, std::string& o3){
    int started = 0;
    o1 = "";
    o2 = "";
    o3 = "";
    for(int i=0; i<a.length(); i++){
        if(a[i] == "_" || (a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0' && a[i] <= '9')){
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

std::string toHtmlTag(std::string s){
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
        else if(a[i] >= '0' && a[i] <= '9'){
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

void addObjList(const std::map<std::string, std::string, compareObj>& x, std::string& s){
    for(std::map<std::string, std::string, compareObj>::const_iterator i = x.begin(); i != x.end(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj((*i).first, s1, s2, s3);
        s += "<p class=\"label\">&#x9;";
        s += s1;
        s += ("<a class=\"link\" href=\"" + toHtmlTag((*i).first) + "\">" + s2 + "</a>");
        s += s3;
        s += "</p>\n";
    }
}

void addObjFull(const std::map<std::string, std::string, compareObj>& x, std::string& s){
    for(std::map<std::string, std::string, compareObj>::const_iterator i = x.begin(); i != x.end(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj((*i).first, s1, s2, s3);
        s += ("<h2 class=\"halftitle\" id=\"" + toHtmlTag((*i).first).substr(1) + "\">&#x9;");
        s += s1;
        s += ("<a class=\"link\" href=\"" + toHtmlTag((*i).first) + "\">" + s2 + "</a>");
        s += s3;
        s += "</h2>\n";
        s += ("<p class=\"label\">" + formatDescription((*i).second) + "<br>&nbsp;</p>\n");
    }
}

std::string genClassInfo(std::string input){
    std::string className;
    std::string classDescription;
    std::string include;
    std::string parentClass;
    std::vector<std::string> childrenClasses;
    std::pair<std::string, std::string> enumName;
    std::string enumDescription;
    std::map<std::string, std::string, StringComparator> enumFlags;
    std::map<std::string, std::string, StringComparator> memberProperties;
    std::map<std::string, std::string, StringComparator> memberFunctions;
    std::map<std::string, std::string, StringComparator> staticProperties;
    std::map<std::string, std::string, StringComparator> staticFunctions;
    std::map<std::string, std::string, StringComparator> memberReimplemented;
    std::map<std::string, std::string, StringComparator> staticReimplemented;
    std::string laterDescription;
    std::map<std::string, std::string, StringComparator> detailedDescription;
    
    std::stringstream stream(input);
    std::string s;
    while(std::getline(stream, s)){
        std::string type;
        std::string value;
        std::string sd;
        if(s.length() == 0){continue;}
        else if(s[0] != '#'){return ("invalid syntax: " + s);}
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
            enumFlags.emplace(value, sd);
        }
        else if(type == "p"){
            std::getline(stream, sd);
            memberProperties.emplace(value, sd);
        }
        else if(type == "f"){
            std::getline(stream, sd);
            memberFunctions.emplace(value, sd);
        }
        else if(type == "sp"){
            std::getline(stream, sd);
            staticProperties.emplace(value, sd);
        }
        else if(type == "sf"){
            std::getline(stream, sd);
            staticFunctions.emplace(value, sd);
        }
        else if(type == "r"){
            std::getline(stream, sd);
            memberReimplemented.emplace(value, sd);
        }
        else if(type == "sr"){
            std::getline(stream, sd);
            staticReimplemented.emplace(value, sd);
        }
        else if(type == "d"){laterDescription = value;}
        else if(type == "dt"){
            std::getline(stream, sd);
            detailedDescription.emplace(value, sd);
        }
        else{return ("invalid type: " + type);}
    }
    
    std::string output = "";
    output += ("<h1 class=\"title\">" + className + "</h1>\n");
    output += ("<p class=\"label\">" + formatDescription(classDescription) + "</p>\n");
    output += ("<p class=\"label\">&nbsp;<br>&#x9;header: #include\" " + include + "\"</p>\n");
    if(parentClass == ""){output += "<p class=\"label\">&#x9;inherits: none</p>\n";}
    else{output += ("<p class=\"label\">&#x9;inherits: <a class=\"link\" href=\"./classinfo.html?classname=" + toLowerCase(parentClass) + "\">" + parentClass + "</a></p>\n");}
    if(childrenClasses.size() > 0){
        output += "<p class=\"label\">&#x9;inherited by:</p>\n";
        for(int i=0; i<childrenClasses.size(); i++){
            output += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"./classinfo.html?classname=" + toLowerCase(childrenClasses[i]) + "\">" + childrenClasses[i] + "</a></p>\n");
        }
    }
    output += "<p class=\"label\">&#x9;<a class=\"link\" href=\"#moreinfo\">More information...</a></p>";
    output += "<p class=\"label\">&nbsp;</p>";
    
    if(enumName != ""){
        output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
        output += "<h2 class=\"halftitle\">Attached Enum</h2>\n";
        output += ("<p class=\"label\">&#x9;enum <a class=\"link\" href=\"#enum" + toLowerCase(enumName) + "\">" +  enumName + "</a></p>\n");
        for(std::map<std::string, std::string, StringComparator>::const_iterator i = enumFlags.begin(); i != enumFlags.end(); i++){
            output += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"#flag" + toLowerCase((*i).first) + "\">" + (*i).first + "</a></p>\n");
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
    for(std::map<std::string, std::string, compareObj>::const_iterator i = detailedDescription.begin(); i != detailedDescription.end(); i++){
        output += ("<h2 class=\"halftitle\">" + (*i).first + "</h2>\n");
        output += ("<p class=\"label\">" + formatDescription((*i).second) + "</p>\n");
    }
    output += "<p class=\"label\">&nbsp;</p>";
    output += "<div style=\"width: 100%; height: 0.25em; background-color: var(--c4);\"></div>\n";
    
    if(enumName != ""){
        output += ("<h2 class=\"halftitle\" id=\"enum" + toLowerCase(enumName) + "\"><a class=\"link\" href=\"#enum" + toLowerCase(enumName) + "\">" + enumName + "</a></h2>\n");
        output += ("<p class=\"label\">" + formatDescription(enumDescription) + "<br>&nbsp;</p>\n");
        for(std::map<std::string, std::string, StringComparator>::const_iterator i = enumFlags.begin(); i != enumFlags.end(); i++){
            output += ("<h2 class=\"halftitle\" id=\"flag" + toLowerCase((*i).first) + "\">" + enumName + " <a class=\"link\" href=\"#flag" + toLowerCase((*i).first) + "\">" + (*i).first + "</a></h2>\n");
            output += ("<p class=\"label\">" + formatDescription((*i).second) + "<br>&nbsp;</p>\n");
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
