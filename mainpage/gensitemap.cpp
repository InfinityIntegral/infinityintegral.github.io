std::string genSiteMap(std::string input){
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
            std::string link = "\"./classinfo?classname=" + sl + "\"";
            output += ("<a class=\"link\" href=" + link + ">");
            output += s;
            output += "</a>";
        }
    }
    output += "\n</p>";
    return output;
}
