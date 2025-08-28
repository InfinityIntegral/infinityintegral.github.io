#include <string>
#include <sstream>
#include <vector>

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

std::string toLowerCase(std::string s){
    std::string output = "";
    for(int i=0; i<s.length(); i++){
        if(s[i] >= 'a' && s[i] <= 'z'){output += s[i];}
        else if(s[i] >= 'A' && s[i] <= 'Z'){output += (s[i] - 'A' + 'a');}
        else if(s[i] >= '0' && s[i] <= '9'){output += s[i];}
    }
    return output;
}

void splitObj(std::string a, std::string& o1, std::string& o2, std::string& o3){
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

void addObjList(const std::vector<std::string>& x, std::string& s, std::string cn){
    for(int i=0; i<x.size(); i++){
        std::string s1;
        std::string s2;
        std::string s3;
        splitObj(x.at(i), s1, s2, s3);
        s += "<p class=\"label\">&#x9;";
        s += s1;
        s += ("<a class=\"link\" href=\"./classinfo.html?classname=" + toLowerCase(cn) + toHtmlTag(x.at(i)) + "\">" + cn + "::" + s2 + "</a>");
        s += s3;
        s += "</p>\n";
    }
}

ClassInfo genClassInfo(std::string input){
    ClassInfo output;
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
            output.className = value;
            std::getline(stream, sd);
            document.title = (value + " list");
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

std::vector<ClassInfo> getClassInfoList(std::string input){
    std::vector<ClassInfo> reversedList;
    std::string currentClass = input;
    while(currentClass != ""){
        // next line of code uses getTextFile which is already implemented somewhere else, and it already uses JS async-await syntax
        ClassInfo nextClass = genClassInfo(await getTextFile("docs/" + toLowerCase(currentClass) + ".html"));
        reversedList.push_back(nextClass);
        currentClass = nextClass.parentName;
    }
    std::vector<ClassInfo> output;
    for(int i=reversedList.size()-1; i>=0; i--){
        output.push_back(reversedList.at(i));
    }
    return output;
}

std::string genFunctionList(std::string input){
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
            output += ("<p class=\"label\">&#x9;enum <a class=\"link\" href=\"./classinfo.html?classname=" + toLowerCase(currentClass.className) + "#enum" + toLowerCase(currentClass.enumName) + "\">" + currentClass.className + "::" +  currentClass.enumName + "</a></p>\n");
            for(int i=0; i<currentClass.enumFlags.size(); i++){
                output += ("<p class=\"label\">&#x9;&#x9;<a class=\"link\" href=\"./classinfo.html?classname=" + toLowerCase(currentClass.className) + "#flag" + toLowerCase(currentClass.enumFlags.at(i)) + "\">" + currentClass.className + "::" + currentClass.enumFlags.at(i) + "</a></p>\n");
            }
        }

        if(currentClass.memberProperties.size() > 0){
            pUsed = true;
            addObjList(currentClass.memberProperties, pHTML, currentClass.className);
        }
        if(currentClass.memberFunctions.size() > 0){
            fUsed = true;
            addObjList(currentClass.memberFunctions, fHTML, currentClass.className);
        }
        if(currentClass.memberReimplemented.size() > 0){
            rUsed = true;
            addObjList(currentClass.memberReimplemented, rHTML, currentClass.className);
        }
        if(currentClass.staticProperties.size() > 0){
            spUsed = true;
            addObjList(currentClass.staticProperties, spHTML, currentClass.className);
        }
        if(currentClass.staticFunctions.size() > 0){
            sfUsed = true;
            addObjList(currentClass.staticFunctions, sfHTML, currentClass.className);
        }
        if(currentClass.staticReimplemented.size() > 0){
            srUsed = true;
            addObjList(currentClass.staticReimplemented, srHTML, currentClass.className);
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
