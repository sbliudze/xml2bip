#include <iostream>
#include <fstream>
#include <string>
#include <functional>  
#include "tinyxml2.h"
#include <unistd.h>
#include <getopt.h>
#include <vector>
 
struct Node_Name
{
    std::vector<std::string> Vector_condition;
    std::vector<std::string> Vector_action;
    std::vector<std::vector<std::string> > Vector_sequence;
    std::vector<std::vector<std::string> > Vector_fallback;
};

int tree_num = -1;

struct Node_Number
{
    int all_num;
    int condition_num;
    int action_num;
    int sequence_num;
    int fallback_num;
};

auto StrEqual = [](const char* str1, const char* str2) -> bool 
{
    return strcmp(str1, str2) == 0;
};

//计算下一层子节点数
auto subChildrenCount = [](const tinyxml2::XMLElement* parent_node) 
{
    int count = 0;
    for (auto node = parent_node->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
    {
        count++;
        std::cout << node->Attribute( "name" ) << "\t" ;
    }
    return count;
};

// 计算所有子节点数
int allsubChildrenCount (tinyxml2::XMLElement* parent_node,int &all_sub_num) 
{
    for (tinyxml2::XMLElement* currentele = parent_node->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    all_sub_num ++;
        tinyxml2::XMLElement* tmpele = currentele;
        const char* name = tmpele->Name();
	    if (!tmpele->NoChildren())
		    allsubChildrenCount(tmpele,all_sub_num);
	}
    return all_sub_num;
};

//给定节点的子节点中的复合节点的个数
auto subChildHaveComposite = [](const tinyxml2::XMLElement* parent_node) 
{
    int count = 0;
    for (auto node = parent_node->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
    {
        if(!strcmp(node->Name(),"Sequence") || !strcmp(node->Name(),"Fallback"))
        {
            count++;
        }
    }
    return count;
};

void conditionPrint(const char* node_name,int &tree_num,tinyxml2::XMLElement* tree_node,std::string &output_filename)
{
    std::ofstream outfile(output_filename,std::ios::app);

    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;

    outfile << "\tatomic type " << tree_node->Attribute( "name" ) << std::endl;
    // outfile << "\t\tdata int x\n";
    outfile << "\t\texport port sync success" << "\n";
    outfile << "\t\texport port sync failure" << "\n";
    outfile << "\t\texport port sync tick" << "\n";
    outfile << "\t\texport port sync running" << "\n";

    outfile << "\t\tplace l0,l1 " << "\n";
    outfile << "\t\tinitial to l0 " << "\n";
    outfile << "\t\ton tick from l0 to l1 " << "\n";
    outfile << "\t\ton success from l1 to l0 " << "\n";
    outfile << "\t\ton failure from l1 to l0 " << "\n";
    outfile << "\tend" << "\n";

    outfile << std::endl;
    outfile.close();
}

void actionPrint(const char* node_name,int &tree_num,tinyxml2::XMLElement* tree_node,std::string &output_filename)
{
    std::ofstream outfile(output_filename,std::ios::app);
 
    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;

    outfile << "\tatomic type " << tree_node->Attribute( "name" ) << std::endl;
    // outfile << "\t\tdata int x\n";
    outfile << "\t\texport port sync success" << "\n";
    outfile << "\t\texport port sync failure" << "\n";
    outfile << "\t\texport port sync tick" << "\n";
    outfile << "\t\texport port sync running" << "\n";

    outfile << "\t\tplace l0,l1 " << "\n";
    outfile << "\t\tinitial to l0 " << "\n";
    outfile << "\t\ton tick from l0 to l1 do {}" << "\n";
    outfile << "\t\ton success from l1 to l0 " << "\n";
    outfile << "\t\ton failure from l1 to l0 " << "\n";
    outfile << "\t\ton running from l1 to l0 provided(0)" << "\n";
    outfile << "\tend" << "\n";

    outfile << std::endl;
    outfile.close();
}

void fallbackPrint(const char* node_name,std::vector<std::string>& seq,int &tree_num,tinyxml2::XMLElement* tree_node,std::string &output_filename)
{
    std::ofstream outfile(output_filename,std::ios::app);

    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;
    outfile << "\tatomic type " << tree_node->Attribute( "name" ) << std::endl;
    outfile << "\t\texport port sync success" << "\n";
    outfile << "\t\texport port sync failure" << "\n";
    outfile << "\t\texport port sync tick" << "\n";
    outfile << "\t\texport port sync running" << "\n";
    
    int sub_nums = subChildrenCount(tree_node);//下一层子节点
    for(int i =1;i <= sub_nums;i++)
    {
        outfile << "\t\texport port sync success" << i << "\n";
        outfile << "\t\texport port sync failure" << i << "\n";
        outfile << "\t\texport port sync tick" << i << "\n";
        outfile << "\t\texport port sync running" << i << "\n";
    }

    outfile << "\t\tplace l0";
    for(int i =0;i < sub_nums;i++)
        outfile << ",l" << i+1 << ",l" << i+1 << i+1;
    outfile << ",l" << sub_nums+1 << ",lf,lr" << std::endl;

    outfile << "\t\tinitial to l0" << std::endl;


    outfile << "\t\ton tick from l0 to l1\n";
    outfile << "\t\ton failure from l" << sub_nums +1 << " to l0\n";
    outfile << "\t\ton success from lf to l0\n";
    outfile << "\t\ton running from lr to l0\n";

    for(int i =0;i < sub_nums;i++)
    {
        outfile << "\t\ton tick" << i+1 << " from l" << i+1 << " to l" << i+1 << i+1 << "\n";
        outfile << "\t\ton failure" << i+1 << " from l" << i+1 << i+1 << " to l" << i +2 << "\n";
        outfile << "\t\ton success" << i+1 << " from l" << i+1 << i+1 << " to lf\n";
        outfile << "\t\ton running" << i+1 << " from l" << i+1 << i+1 << " to lr\n";
    }

    outfile <<"\tend\n\n";
    outfile.close();
}

void sequencePrint(const char* node_name,std::vector<std::string>& seq,int &tree_num,tinyxml2::XMLElement* tree_node,std::string &output_filename)
{
    std::ofstream outfile(output_filename,std::ios::app);

    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;
    outfile << "\tatomic type " << tree_node->Attribute( "name" ) << std::endl;
    outfile << "\t\texport port sync success" << "\n";
    outfile << "\t\texport port sync failure" << "\n";
    outfile << "\t\texport port sync tick" << "\n";
    outfile << "\t\texport port sync running" << "\n";
    
    int sub_nums = subChildrenCount(tree_node);//下一层子节点
    for(int i =1;i <= sub_nums;i++)
    {
        outfile << "\t\texport port sync success" << i << "\n";
        outfile << "\t\texport port sync failure" << i << "\n";
        outfile << "\t\texport port sync tick" << i << "\n";
        outfile << "\t\texport port sync running" << i << "\n";
    }

    outfile << "\t\tplace l0";
    for(int i =0;i < sub_nums;i++)
        outfile << ",l" << i+1 << ",l" << i+1 << i+1;
    outfile << ",l" << sub_nums+1 << ",lf,lr\n";
    outfile << "\t\tinitial to l0" << std::endl;

    outfile << "\t\ton tick from l0 to l1\n";
    outfile << "\t\ton success from l" << sub_nums +1 << " to l0\n";
    outfile << "\t\ton failure from lf to l0\n";
    outfile << "\t\ton running from lr to l0\n";

    for(int i =0;i < sub_nums;i++)
    {
        outfile << "\t\ton tick" << i+1 << " from l" << i+1 << " to l" << i+1 << i+1 << "\n";
        outfile << "\t\ton success" << i+1 << " from l" << i+1 << i+1 << " to l" << i +2 << "\n";
        outfile << "\t\ton failure" << i+1 << " from l" << i+1 << i+1 << " to lf\n";
        outfile << "\t\ton running" << i+1 << " from l" << i+1 << i+1 << " to lr\n";
    }

    outfile <<"\tend\n\n";
    outfile.close();
}

// 计算各类节点的个数
void node_num(tinyxml2::XMLElement * element,struct Node_Number &getNodeNb)
{
    for (tinyxml2::XMLElement* currentele = element->FirstChildElement();
        currentele;
        currentele = currentele->NextSiblingElement())
	{
	    tree_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        if(!strcmp(tmpele->Name(),"Condition"))
        {
            getNodeNb.condition_num ++;
        }

        if(!strcmp(tmpele->Name(),"Action"))
        {
            getNodeNb.action_num ++;
        }

        if(!strcmp(tmpele->Name(),"Fallback"))
        {
            getNodeNb.fallback_num ++;
        }

        if(!strcmp(tmpele->Name(),"Sequence"))
        {
            getNodeNb.sequence_num ++;
        }
        
	    if (!tmpele->NoChildren())
		    node_num(tmpele,getNodeNb);
	}
}

void GetEleValue_for_name(tinyxml2::XMLElement * element,struct Node_Name &getNodeName)
{

    for (tinyxml2::XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    tree_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        if(!strcmp(tmpele->Name(),"Condition"))
        {
            getNodeName.Vector_condition.push_back(tmpele->Attribute( "name" ));
        }

        if(!strcmp(tmpele->Name(),"Action"))
        {
            getNodeName.Vector_action.push_back(tmpele->Attribute( "name" ));
        }

        if(!strcmp(tmpele->Name(),"Fallback"))
        {
            std::vector<std::string> numFall;
            numFall.push_back(tmpele->Attribute( "name" ));
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numFall.push_back(node->Attribute( "name" ));
            }
            getNodeName.Vector_fallback.push_back(numFall);
        }

        if(!strcmp(tmpele->Name(),"Sequence"))
        {
            std::vector<std::string> numSeq;
            numSeq.push_back(tmpele->Attribute( "name" ));
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numSeq.push_back(node->Attribute( "name" ));
            }
            getNodeName.Vector_sequence.push_back(numSeq);
        }
        
	    if (!tmpele->NoChildren())
		    GetEleValue_for_name(tmpele,getNodeName);
	}
}

void GetEleValue(tinyxml2::XMLElement * element,std::string &output_filename,std::vector<std::string> &numCom)
{
    for (tinyxml2::XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    tree_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        const char* name = tmpele->Name();      
        if((!strcmp(name,"Sequence") || !strcmp(name,"Fallback")))
        {
            // std::cout << "I " << tmpele->Name() << " have " <<subChildrenCount(tmpele) << " nums sub tree!  and sub chaild have " 
            //                                                                            << subChildHaveComposite(tmpele) << " nums Composite"<< std::endl;
                numCom.push_back(tmpele->Attribute( "name" ));
        }        

        if(!strcmp(tmpele->Name(),"Condition"))
        {
            conditionPrint(tmpele->Attribute( "name" ),tree_num,tmpele,output_filename);
        }

        if(!strcmp(tmpele->Name(),"Action"))
        {
            actionPrint(tmpele->Attribute( "name" ),tree_num,tmpele,output_filename);
        }

        if(!strcmp(tmpele->Name(),"Fallback"))
        {
            std::vector<std::string> numFall;
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numFall.push_back(node->Attribute( "name" ));
            }
            std::cout << std::endl;
            fallbackPrint(tmpele->Attribute( "name" ),numFall,tree_num,tmpele,output_filename);
        }

        if(!strcmp(tmpele->Name(),"Sequence"))
        {
            std::vector<std::string> numSeq;
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numSeq.push_back(node->Attribute( "name" ));
            }
            std::cout << std::endl;
            sequencePrint(tmpele->Attribute( "name" ),numSeq,tree_num,tmpele,output_filename);
        }

	    if (!tmpele->NoChildren())
		    GetEleValue(tmpele,output_filename,numCom);
	}
}

void main_Model_Print(struct Node_Name &getNodeName,struct Node_Number &getNodeNb,tinyxml2::XMLElement* xml_root,std::string &output_filename,std::vector<std::string> &numCom)
{
    std::ofstream outfile(output_filename,std::ios::app);
    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;
    outfile << "model main" << std::endl;
    outfile << "\t" << "port type sync" << std::endl;
    
    outfile << "\t" << "connector type rendezvous(sync p1,sync p2)" << "\n"
            << "\t\t" << "define [p1 p2]" << "\n"
            << "\t" << "end" << "\n" << std::endl;

    outfile << "\t" << "connector type single(sync p)" << "\n"
            << "\t\t" << "define [p]" << "\n"
            << "\t" << "end" << "\n" << std::endl;  

    GetEleValue(xml_root,output_filename,numCom);

    outfile << "\t" << "compound type system" << std::endl;

    std::cout << "\ncondition num is " << getNodeNb.condition_num << std::endl;
    if(getNodeNb.condition_num>0)
    {
        
        for(int i =0;i< getNodeName.Vector_condition.size();i++)
        {
            outfile << "\t\t" << "component " << getNodeName.Vector_condition[i] << " " << getNodeName.Vector_condition[i] << "_c" << std::endl;
            
        }
        outfile << std::endl;
    }
    std::cout << "action num is " << getNodeNb.action_num << std::endl;
    if(getNodeNb.action_num>0)
    {
        
        for(int i =0;i< getNodeName.Vector_action.size();i++)
        {
            outfile << "\t\t" << "component " << getNodeName.Vector_action[i] << " " << getNodeName.Vector_action[i] << "_c" << std::endl;
        }
        outfile << std::endl;
    }
    std::cout << "sequence num is " << getNodeNb.sequence_num << std::endl;
    if(getNodeNb.sequence_num>0)
    {
        
        for(int i =0;i< getNodeName.Vector_sequence.size();i++)
        {
            outfile << "\t\t" << "component " << getNodeName.Vector_sequence[i][0] << " " << getNodeName.Vector_sequence[i][0] << "_c" << std::endl;
        }
        outfile << "\n";
    }
    std::cout << "fallback num is " << getNodeNb.fallback_num << std::endl;
    if(getNodeNb.fallback_num>0)
    {
        
        for(int i =0;i< getNodeNb.fallback_num;i++)
        {
            outfile << "\t\t" << "component " << getNodeName.Vector_fallback[i][0] << " " << getNodeName.Vector_fallback[i][0] << "_c" << std::endl;
        }
        outfile << std::endl;
    }

    if(getNodeNb.sequence_num>0)
    {
        outfile << "\n";
        for(int i =0;i< getNodeName.Vector_sequence.size();i++)
        {
            for(int j = 1;j< getNodeName.Vector_sequence[i].size();j++)
            {
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_sequence[i][0] << "_c" << "_to_" << getNodeName.Vector_sequence[i][j] << "_c" << "_tick("  << getNodeName.Vector_sequence[i][0] << "_c" << ".tick"<< j << "," << getNodeName.Vector_sequence[i][j] << "_c" << ".tick)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_sequence[i][0] << "_c" << "_to_" << getNodeName.Vector_sequence[i][j] << "_c" << "_success("  << getNodeName.Vector_sequence[i][0] << "_c" << ".success" << j << "," << getNodeName.Vector_sequence[i][j] << "_c" << ".success)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_sequence[i][0] << "_c" << "_to_" << getNodeName.Vector_sequence[i][j] << "_c" << "_failure("  << getNodeName.Vector_sequence[i][0] << "_c" << ".failure" << j << "," << getNodeName.Vector_sequence[i][j] << "_c" << ".failure)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_sequence[i][0] << "_c" << "_to_" << getNodeName.Vector_sequence[i][j] << "_c" << "_running("  << getNodeName.Vector_sequence[i][0] << "_c" << ".running" << j << "," << getNodeName.Vector_sequence[i][j] << "_c" << ".running)" <<"\n";            
            }
        }

    }

    if(getNodeNb.fallback_num>0)
    {
        outfile << "\n";
        for(int i =0;i< getNodeName.Vector_fallback.size();i++)
        {
            for(int j = 1;j< getNodeName.Vector_fallback[i].size();j++)
            {
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_fallback[i][0] << "_c" << "_to_" << getNodeName.Vector_fallback[i][j] << "_c" << "_tick("  << getNodeName.Vector_fallback[i][0] << "_c" << ".tick"<< j << "," << getNodeName.Vector_fallback[i][j] << "_c" << ".tick)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_fallback[i][0] << "_c" << "_to_" << getNodeName.Vector_fallback[i][j] << "_c" << "_success("  << getNodeName.Vector_fallback[i][0] << "_c" << ".success" << j << "," << getNodeName.Vector_fallback[i][j] << "_c" << ".success)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_fallback[i][0] << "_c" << "_to_" << getNodeName.Vector_fallback[i][j] << "_c" << "_failure("  << getNodeName.Vector_fallback[i][0] << "_c" << ".failure" << j << "," << getNodeName.Vector_fallback[i][j] << "_c" << ".failure)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << getNodeName.Vector_fallback[i][0] << "_c" << "_to_" << getNodeName.Vector_fallback[i][j] << "_c" << "_running("  << getNodeName.Vector_fallback[i][0] << "_c" << ".running" << j << "," << getNodeName.Vector_fallback[i][j] << "_c" << ".running)" <<"\n";
            }
        }

    }

    outfile << std::endl; 
    outfile << "\t\t" << "connector single " << numCom[0] << "_tick("  << numCom[0] << "_c" << ".tick" << ")" << std::endl;
    outfile << "\t\t" << "connector single " << numCom[0] << "_success("  << numCom[0] << "_c" << ".success" << ")" << std::endl;
    outfile << "\t\t" << "connector single " << numCom[0] << "_failure("  << numCom[0] << "_c" << ".failure" << ")" << std::endl;
    outfile << "\t\t" << "connector single " << numCom[0] << "_running("  << numCom[0] << "_c" << ".running" << ")" << std::endl;

    outfile << "\tend\n\n";
    outfile << "\tcomponent system sys\n" << std::endl;
    outfile.close();
}



std::string CharToStr(char * contentChar)
{
	std::string tempStr;
	for (int i=0;contentChar[i]!='\0';i++)
	{
		tempStr+=contentChar[i];
	}
	return tempStr;
}

int main( int argc, char* argv[] )
{
	
    std::string input_filename,output_filename;
    
    const char * const short_options = "i:o:";
    int option;

    const struct option long_options[] = 
    {
        { "input",    0, NULL, 'i' },
        { "output", 0, NULL, 'o' }
    };

    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) 
    {
		switch (option) 
        {
			case 'i': input_filename = std::string(optarg); break;
            case 'o': output_filename = std::string(optarg); break;
		}
	}

    struct Node_Name getNodeName;
    struct Node_Number getNodeNb = {0,0,0,0,0};
    std::vector<std::string> numCom;

    tinyxml2::XMLDocument doc;
	if ( doc.LoadFile(input_filename.c_str()) )
	{
		doc.PrintError();
		exit( 1 );
	}
 
	// 根元素
	tinyxml2::XMLElement* xml_root = doc.RootElement();
    
    // int all_sub_num = -1;
    // int root_sub_num = allsubChildrenCount(xml_root,all_sub_num);
    // std::cout << "root_sub_num is " << root_sub_num << std::endl;
    
    if (!xml_root || !StrEqual(xml_root->Name(), "root"))
    {
        throw std::runtime_error("The XML must have a root node called <root>");
    }

    remove(output_filename.c_str());
    std::ofstream outfile(output_filename,std::ios::app);

    node_num(xml_root,getNodeNb);
    GetEleValue_for_name(xml_root,getNodeName);
    main_Model_Print(getNodeName,getNodeNb,xml_root,output_filename,numCom);
    outfile << "end" << std::endl; 

    return 0;
}
