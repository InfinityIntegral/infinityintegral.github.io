#include <fstream>
#include <filesystem>
#include <string>

// g++ -o private_api/sitemap.exe private_api/sitemap.cpp && private_api\sitemap.exe

std::string website = "https://infinityintegral.github.io";
std::string rootFolder = "C:\\website";
std::string outputFile = "C:\\website\\sitemap.xml";

int main(){
    std::ofstream output(outputFile);
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output << "<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
    for(std::filesystem::recursive_directory_iterator i = std::filesystem::recursive_directory_iterator(rootFolder); i != std::filesystem::recursive_directory_iterator(); i++){
        if(std::filesystem::is_regular_file(*i) == true){
            std::string path = (*i).path().string();
            for(int j=0; j<path.length(); j++){
                if(path.at(j) == '\\'){path.at(j) = '/';}
            }
            if(path.length() > 5 && path.substr(path.length() - 5, 5) == ".html"){
                output << "<url><loc>" << website << path.substr(rootFolder.length(), path.length() - rootFolder.length() - 5) << "</loc></url>\n";
            }
        }
    }
    output << "</urlset>\n";
}
