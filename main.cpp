#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <map>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"


std::string readFile(std::string filePath){

    std::ifstream infile(filePath,std::ios::in);
    if (! infile.is_open())
    { std::cout << "Error opening file"; exit (1); }

    std::string line;
    std::ostringstream outss;
    while(getline(infile,line)){
        outss<<line<<std::endl;
    }

    return outss.str();
}

struct Token{
    int sentenceid;
    int tokenid;
    std::string word;
    std::string ner;
};

struct Corefer{
    int sentenceid;
    int Cosetid;
    bool isRepresentative;
    int startIndex;
    int endIndex;
    int headIndex;
    std::string text;

};
struct NamedEntityPhrase{
    std::string headstring;
    std::string phrase;
    std::string entityType;
};

typedef std::map<int,NamedEntityPhrase> NEPMAP;
NEPMAP neps;
std::map<int,NEPMAP> senNEP;

void extractNEP(std::vector<Token> tokens,std::map<int,NEPMAP>& senNEP){

    for(int i=0;i<tokens.size();i++){

    }
}

class Phrase;
//Phrase add attribute string NEtype  bool isNE
void addNEtoPhrase(NEPMAP neps,std::map<int,Phrase>&phrases){

}

int main() {


    std::vector<Token> nerTokens;

    std::vector<Corefer> corefers;


    rapidxml::file<> fdoc("cccinput.xml");


    rapidxml::xml_document<> doc;

    doc.parse<0>(fdoc.data());


    rapidxml::xml_node<>* root = doc.first_node();

    rapidxml::xml_node<>* document = root->first_node();

    rapidxml::xml_node<>* sentences = document->first_node("sentences");

    rapidxml::xml_node<>* coreferences = document->first_node("coreference");

    for(auto node = sentences->first_node("sentence");node!= nullptr;node = node->next_sibling()){

        rapidxml::xml_node<>* tokens = node->first_node("tokens");

        for(auto node_token= tokens->first_node("token");node_token!= nullptr;node_token = node_token->next_sibling()){

            Token token;

            token.sentenceid = atoi(node->first_attribute("id")->value());
            token.tokenid = atoi(node_token->first_attribute("id")->value());
            token.word = node_token->first_node("word")->value();
            token.ner = node_token->first_node("NER")->value();

            nerTokens.push_back(token);

        }
    }

    int cosetid=1;
    for(auto node = coreferences->first_node("coreference");node!= nullptr;node=node->next_sibling(),cosetid++){

        for(auto mention_node = node->first_node("mention");mention_node!= nullptr;mention_node=mention_node->next_sibling()){

            Corefer corefer;

            auto res = mention_node->first_attribute("representative");

            if(res!= nullptr){
                corefer.isRepresentative = true;
                std::cout<<res->value()<<std::endl;
            }else{
                corefer.isRepresentative = false;
            }

            corefer.sentenceid = atoi(mention_node->first_node("sentence")->value());
            corefer.Cosetid = cosetid;
            corefer.startIndex = atoi(mention_node->first_node("start")->value());
            corefer.endIndex = atoi(mention_node->first_node("end")->value());
            corefer.headIndex = atoi(mention_node->first_node("head")->value());
            corefer.text = mention_node->first_node("text")->value();

            corefers.push_back(corefer);

        }

    }



    std::cout << "Hello, World!" << std::endl;
    return 0;
}

