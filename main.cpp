#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <locale>

#include <vector>
#include <map>
#include <stdlib.h>
#include <cmath>
#include <codecvt>
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

std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
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
    for(auto it = phrases.begin();it!=phrases.end();it++){
        if(it->second.isArgument== false){
            continue;
        } else{
            int  phead = it->second.head;
            std::string pheadstring = ztree.getNode(phead).word;

            int maxlenlcs = 0;
            NEPMAP::iterator maxlenitor;
            for(auto it2= neps.begin();it2!=neps.end();it2++){
                int neHead = it2->first;
                std::string neHeadstring = it2->second.headstring;

                std::wstring wphead = s2ws(pheadstring);

                std::wstring wneHead = s2ws(neHeadstring);
                if(std::fabs(phead-neHead)<=3&&wphead.back()==wneHead.back()){
                    it->second.entityType = it2->second.entityType;
                    neps.erase(it2);
                    break;
                }else{
                    std::string pContent = it->second.content;
                    std::string neContent = it2->second.phrase;

                    int commonlen = lcs(s2ws(pContent),s2ws(neContent));

                    if(commonlen>maxlenlcs){
                        maxlenlcs = commonlen;
                        maxlenitor = it2;
                    }
                }
            }
            if(maxlenlcs>0){
                neps.erase(maxlenitor);
            }
        }
    }
}




int lcs(std::wstring A,std::wstring B){
    std::vector<std::vector<int> > len;
    len.resize(A.size()+1);
    for(int i = 0;i <= A.size();i++){
        len[i].resize(B.size()+1,0);
    }
    for (int i = 1; i <= A.size(); ++i)
    {
        for (int j = 1; j <= B.size(); ++j)
        {
            if (A[i-1] == B[j-1])
            {
                len[i][j] = len[i-1][j-1] + 1;
            }else if (len[i-1][j] >= len[i][j-1])
            {
                len[i][j] = len[i-1][j];
            }else{
                len[i][j] = len[i][j-1];
            }
        }
    }
    int apos = A.size();
    int bpos = B.size();
    int commonlen = len[apos][bpos];
    int k = commonlen;
    /*common.resize(commonlen);
    while(apos && bpos){
        if(len[apos][bpos] == len[apos-1][bpos] + 1){
            common[--k] = A[--apos];
            --bpos;
        }else if (len[apos-1][bpos] >= len[apos][bpos-1])
        {
            --apos;
        }else{
            --bpos;
        }
    }
    for(int i = 0;i < commonlen;i++){
        cout<<common[i];
    }
    cout<<endl;*/
    return commonlen;
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

