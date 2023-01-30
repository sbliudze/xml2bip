#include <iostream>
#include <fstream>
#include <string>
#include <functional>  
#include "tinyxml2.h"

#include <vector>
 

// function to be called recursively
int tree_num = -1;
int condition_num =0;
int action_num =0;
int sequence_num =0;
int fallback_num =0;
std::vector<std::string> Vector_condition,Vector_action;
std::vector<std::vector<std::string> >Vector_sequence,Vector_fallback;

auto ChildrenCount = [](const tinyxml2::XMLElement* parent_node) 
{
    int count = 0;
    for (auto node = parent_node->FirstChildElement(); node != nullptr;
            node = node->NextSiblingElement())
    {
        count++;
    }
    return count;
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
    // int all_sub_num = 0;
    for (tinyxml2::XMLElement* currentele = parent_node->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    all_sub_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        const int children_count = ChildrenCount(tmpele);
        const char* name = tmpele->Name();
	    if (!tmpele->NoChildren())
		    allsubChildrenCount(tmpele,all_sub_num);
	}
    return all_sub_num;
};

//子树是否有复合节点
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

// 将源文件中的特定字符串替换,内容输出到新文件中
bool FileStringReplace(std::ifstream &instream, std::ofstream &outstream,int &tree_num)
{
	std::string str;
	size_t pos = 0;
	while (getline(instream, str)) // 按行读取
	{
		pos = str.find("**"); // 查找每一行中的"Tom"
		if (pos != std::string::npos)
		{
			str = str.replace(pos, 2, std::to_string(tree_num)); // 将Tom替换为Jerry
			outstream << str << std::endl;
			continue;
		}
		outstream << str << std::endl;
	}
	return true;
}

// void conditionPrint(const char* node_name,int &tree_num)
// {
//     std::ofstream outfile("../mytree.bip",std::ios::app);
//     // outfile.open
//     if(!outfile.is_open())
//         std::cout << "open file faileure" << std::endl;
//     outfile << "MODULE " << node_name << "(tick)" << std::endl;

//     //先写入在
//     std::ofstream condition("../cache/condition_" + std::to_string(tree_num) + ".txt");//,std::ios::app);

//     // std::ofstream outfile("../condition_.txt");
//     std::ifstream condition_file("../condition.txt",std::ios::app);
    
//     FileStringReplace(condition_file,condition,tree_num);

//     std::ifstream condition_in("../cache/condition_" + std::to_string(tree_num) + ".txt");//,std::ios::app);
//     std::string temp;
//     if(!condition.is_open())
//         std::cout << "open condition file faileure" << std::endl;
//     while(getline(condition_in, temp))
//     {
//         outfile << temp;
//         outfile << std::endl;  
//     }

//     outfile << std::endl;
//     outfile.close();
// }

// void actionPrint(const char* node_name,int &tree_num)
// {
//     std::ofstream outfile("../mytree.bip",std::ios::app);
//     // outfile.open
//     if(!outfile.is_open())
//         std::cout << "open file faileure" << std::endl;
//     outfile << "MODULE " << node_name << "(tick)"  << std::endl;

    
//     //先写入在
//     std::ofstream action("../cache/action_" + std::to_string(tree_num) + ".txt");//,std::ios::app);

//     // std::ofstream outfile("../condition_.txt");
//     std::ifstream action_file("../action.txt",std::ios::app);
    
//     FileStringReplace(action_file,action,tree_num);

//     std::ifstream action_in("../cache/action_" + std::to_string(tree_num) + ".txt");//,std::ios::app);
//     std::string temp;
//     if(!action.is_open())
//         std::cout << "open action file faileure" << std::endl;
//     while(getline(action_in, temp))
//     {
//         outfile << temp;
//         outfile << std::endl;  
//     }

//     outfile << std::endl;
//     outfile.close();
// }

// void fallbackPrint(const char* node_name,std::vector<std::string>& seq,int &tree_num,tinyxml2::XMLElement* tree_node)
// {
//     std::ofstream outfile("../mytree.bip",std::ios::app);
//     // outfile.open
//     if(!outfile.is_open())
//         std::cout << "open file faileure" << std::endl;
//     outfile << "MODULE " << node_name << "(para_tick)" << std::endl;
//     outfile << "VAR" << "\n";
//     int sub_nums = subChildrenCount(tree_node);//下一层子节点

//     // outfile << "\t" << "tick:{" << tree_num ;
//     int all_sub_trees = tree_num;
//     std::vector<int> No_tree;
//     for (auto node = tree_node->FirstChildElement();node != nullptr;node = node->NextSiblingElement())
//     {
//         int sub_num = 0;
//         all_sub_trees  = all_sub_trees + 1;   
//         // outfile << ","  << all_sub_trees;
//         No_tree.push_back(all_sub_trees);
//         all_sub_trees = all_sub_trees + allsubChildrenCount(node,sub_num);
//     }
//     // outfile << "}; \n";
//     outfile << "\n";

//     outfile << "\t" << "status:{idle,success,failure};" << std::endl;
//     outfile << "\t" << "state:{sta_init,succeed,failed";

//     for(std::vector<int>::iterator it = No_tree.begin();it != No_tree.end();it++)
//     {
//         outfile << ",tick" << *it;
//     }

//     outfile << "}; \n";

//     int seq_num = 0;
//     for(std::vector<std::string>::iterator it = seq.begin();it != seq.end();it++)
//     {
//         outfile << "\t" << "s" << No_tree.at(seq_num) << ":" << *it << "(tick);" << std::endl;
//         seq_num ++;
//     }
//     outfile << "ASSIGN" << "\n"
//             << "\t" << "init(status) := idle;" << "\n"
//             << "\t" << "init(state) := sta_init;" << "\n"
//             << "\t" << "init(tick) := " << tree_num << ";\n";
        
//     outfile << "\t" << "next(state) :=" 
//             << "\n\t\t" << "case\n";

//     outfile << "\t\t\t" << "state = sta_init: tick" << tree_num+1 << ";\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
         
//          outfile << "\t\t\t" << "state = tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = failure : ";
//          if(i +1 == sub_nums)
//          {
//              outfile << "failed;\n";
//          }
//          else
//          {
//              outfile << "tick" << No_tree.at(i+1) << ";\n";
//          }
//         outfile << "\t\t\t" << "state = tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = success :succeed;\n";

//     }
//     outfile << "\t\t\t" << "state = failed :sta_init; \n"
//             << "\t\t\t" << "state = succeed :sta_init; \n"
//             << "\t\t\t" << "TRUE:state;\n" 
//             << "\t\t" << "esac;\n";

//     outfile << "\t" << "next(status) := \n"
//             << "\t\t" << "case\n" 
//             << "\t\t\t" << "state = succeed : success; \n"
//             << "\t\t\t" << "state = failed : failure; \n"
//             << "\t\t\t" << "TRUE:idle;\n" 
//             << "\t\t" << "esac;\n";
//     outfile << "\tnext(tick):=\n"
//             << "\t\t" << "case\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
//          outfile << "\t\t\t" << "state = tick" <<  No_tree.at(i) << " : " <<  No_tree.at(i) << ";"<<"\n";
//     }
//     outfile << "\t\t\t" << "TRUE:tick;\n"
//             << "\t\t" << "esac;\n";

//     outfile << std::endl;
//     outfile.close();
// }

// void sequencePrint(const char* node_name,std::vector<std::string>& seq,int &tree_num,tinyxml2::XMLElement* tree_node)
// {
//     std::ofstream outfile("../mytree.bip",std::ios::app);
//     // outfile.open
//     if(!outfile.is_open())
//         std::cout << "open file faileure" << std::endl;
//     outfile << "MODULE " << node_name << "(para_tick)" << std::endl;
//     outfile << "VAR" << "\n";
//     int sub_nums = subChildrenCount(tree_node);//下一层子节点

//     // outfile << "\t" << "tick:{" << tree_num ;
//     int all_sub_trees = tree_num;
//     std::vector<int> No_tree;
//     for (auto node = tree_node->FirstChildElement();node != nullptr;node = node->NextSiblingElement())
//     {
//         int sub_num = 0;
//         all_sub_trees  = all_sub_trees + 1;   
//         // outfile << ","  << all_sub_trees;
//         No_tree.push_back(all_sub_trees);
//         all_sub_trees = all_sub_trees + allsubChildrenCount(node,sub_num);
//     }
//     // outfile << "}; \n";
//     outfile << "\n";

//     outfile << "\t" << "status:{idle,success,failure};" << std::endl;
//     outfile << "\t" << "state:{sta_init,succeed,failed";

//     for(std::vector<int>::iterator it = No_tree.begin();it != No_tree.end();it++)
//     {
//         outfile << ",tick" << *it;
//     }

//     outfile << "}; \n";

//     int seq_num = 0;
//     for(std::vector<std::string>::iterator it = seq.begin();it != seq.end();it++)
//     {
//         outfile << "\t" << "s" << No_tree.at(seq_num) << ":" << *it << "(tick);" << std::endl;
//         seq_num ++;
//     }
//     outfile << "ASSIGN" << "\n"
//             << "\t" << "init(status) := idle;" << "\n"
//             << "\t" << "init(state) := sta_init;" << "\n"
//             << "\t" << "init(tick) := " << tree_num << ";\n";
        
//     outfile << "\t" << "next(state) :=" 
//             << "\n\t\t" << "case\n";

//     outfile << "\t\t\t" << "state =  sta_init: tick" << tree_num+1 << ";\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
         
//          outfile << "\t\t\t" << "state =  tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = success : ";
//          if(i +1 == sub_nums)
//          {
//              outfile << "succeed;\n";
//          }
//          else
//          {
//              outfile << "tick" << No_tree.at(i+1) << ";\n";
//          }
//         outfile << "\t\t\t" << "state =  tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = failure : failed;\n";

//     }
//     outfile << "\t\t\t" << "state = failed : sta_init; \n"
//             << "\t\t\t" << "state = succeed : sta_init; \n"
//             << "\t\t\t" << "TRUE:state;\n" 
//             << "\t\t" << "esac;\n";

//     outfile << "\t" << "next(status) := \n"
//             << "\t\t" << "case\n" 
//             << "\t\t\t" << "state = succeed : success; \n"
//             << "\t\t\t" << "state = failed : failure; \n"
//             << "\t\t\t" << "TRUE:idle;\n" 
//             << "\t\t" << "esac;\n";
//     outfile << "\tnext(tick):=\n"
//             << "\t\t" << "case\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
//          outfile << "\t\t\t" << "state = tick" <<  No_tree.at(i) << " : " <<  No_tree.at(i) << ";"<<"\n";
//     }
//     outfile << "\t\t\t" << "TRUE:tick;\n"
//             << "\t\t" << "esac;\n";

//     outfile << std::endl;
//     outfile.close();
// }

// void parallelPrint(const char* node_name,std::vector<std::string>& seq,int &tree_num,tinyxml2::XMLElement* tree_node)
// {
//     std::ofstream outfile("../mytree.bip",std::ios::app);
//     // outfile.open
//     if(!outfile.is_open())
//         std::cout << "open file faileure" << std::endl;
//     outfile << "MODULE " << node_name << "(para_tick)" << std::endl;
//     outfile << "VAR" << "\n";
//     int sub_nums = subChildrenCount(tree_node);//下一层子节点

//     // outfile << "\t" << "tick:{" << tree_num ;
//     int all_sub_trees = tree_num;
//     std::vector<int> No_tree;
//     for (auto node = tree_node->FirstChildElement();node != nullptr;node = node->NextSiblingElement())
//     {
//         int sub_num = 0;
//         all_sub_trees  = all_sub_trees + 1;   
//         // outfile << ","  << all_sub_trees;
//         No_tree.push_back(all_sub_trees);
//         all_sub_trees = all_sub_trees + allsubChildrenCount(node,sub_num);
//     }
//     // outfile << "}; \n";
//     outfile << "\n";

//     outfile << "\t" << "status:{idle,success,failure};" << std::endl;
//     outfile << "\t" << "state:{sta_init,succeed,failed";

//     for(std::vector<int>::iterator it = No_tree.begin();it != No_tree.end();it++)
//     {
//         outfile << ",tick" << *it;
//     }

//     outfile << "}; \n";

//     int seq_num = 0;
//     for(std::vector<std::string>::iterator it = seq.begin();it != seq.end();it++)
//     {
//         outfile << "\t" << "s" << No_tree.at(seq_num) << ":" << *it << "(tick);" << std::endl;
//         seq_num ++;
//     }
//     outfile << "ASSIGN" << "\n"
//             << "\t" << "init(status) := idle;" << "\n"
//             << "\t" << "init(state) := sta_init;" << "\n"
//             << "\t" << "init(tick) := " << tree_num << ";\n";
        
//     outfile << "\t" << "next(state) :=" 
//             << "\n\t\t" << "case\n";

//     outfile << "\t\t\t" << "state =  sta_init: tick" << tree_num+1 << ";\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
         
//          outfile << "\t\t\t" << "state =  tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = success : ";
//          if(i +1 == sub_nums)
//          {
//              outfile << "succeed;\n";
//          }
//          else
//          {
//              outfile << "tick" << No_tree.at(i+1) << ";\n";
//          }
//         outfile << "\t\t\t" << "state =  tick" << No_tree.at(i) << " & s" << No_tree.at(i) << ".status = failure : failed;\n";

//     }
//     outfile << "\t\t\t" << "state = failed : sta_init; \n"
//             << "\t\t\t" << "state = succeed : sta_init; \n"
//             << "\t\t\t" << "TRUE:state;\n" 
//             << "\t\t" << "esac;\n";

//     outfile << "\t" << "next(status) := \n"
//             << "\t\t" << "case\n" 
//             << "\t\t\t" << "state = succeed : success; \n"
//             << "\t\t\t" << "state = failed : failure; \n"
//             << "\t\t\t" << "TRUE:idle;\n" 
//             << "\t\t" << "esac;\n";
//     outfile << "\tnext(tick):=\n"
//             << "\t\t" << "case\n";

//     for(int i = 0;i < sub_nums;i++)
//     {
//          outfile << "\t\t\t" << "state = tick" <<  No_tree.at(i) << " : " <<  No_tree.at(i) << ";"<<"\n";
//     }
//     outfile << "\t\t\t" << "TRUE:tick;\n"
//             << "\t\t" << "esac;\n";

//     outfile << std::endl;
//     outfile.close();
// }

void node_num(tinyxml2::XMLElement * element)
{

    for (tinyxml2::XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    tree_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        const int children_count = ChildrenCount(tmpele);
        const char* name = tmpele->Name();

        if(!strcmp(tmpele->Name(),"Condition"))
        {
            condition_num ++;
        }

        if(!strcmp(tmpele->Name(),"Action"))
        {
            action_num ++;
        }

        if(!strcmp(tmpele->Name(),"Fallback"))
        {
            fallback_num ++;
        }

        if(!strcmp(tmpele->Name(),"Sequence"))
        {
            sequence_num ++;
        }
        
	    if (!tmpele->NoChildren())
		    node_num(tmpele);
	}
}

void main_Model_Print()
{
    std::ofstream outfile("../mytree.bip",std::ios::app);
    // outfile.open
    if(!outfile.is_open())
        std::cout << "open file faileure" << std::endl;
    outfile << "MODULE main" << std::endl;
    outfile << "\t" << "port type sync" << "\n";
    
    outfile << "\t" << "atomic type condition" << "\n"
            << "\t\t" << "export port sync tick" << "\n"
            << "\t" << "end" << "\n\n";
        
    outfile << "\t" << "atomic type action" << "\n"
            << "\t" << "end" << "\n\n";

    outfile << "\t" << "atomic type  sequence" << "\n"
            << "\t" << "end" << "\n\n";

    outfile << "\t" << "atomic type fallback" << "\n"
            << "\t" << "end" << "\n\n";

    outfile << "\t" << "connector type rendezvous(sync p1,sync p2)" << "\n"
            << "\t\t" << "define [p1 p2]" << "\n"
            << "\t" << "end" << "\n\n";

    outfile << "\t" << "compound type system" << "\n";

    std::cout <<  "condition_num is " << condition_num << std::endl;
    std::cout <<  "action_num is " << action_num << std::endl;
    std::cout <<  "sequence_num is " << sequence_num << std::endl;
    std::cout <<  "fallback_num is " << fallback_num << std::endl;

    //Vector_condition,Vector_action,Vector_sequence,Vector_fallback
    if(condition_num>0)
    {
        outfile << "\t\t" << "component condition " << Vector_condition[0];
        for(int i =1;i< Vector_condition.size();i++)
        {
            outfile << "," << Vector_condition[i];
        }
        outfile << "\n";
    }

    if(action_num>0)
    {
        outfile << "\t\t" << "component condition " << Vector_action[0];
        for(int i =1;i< Vector_action.size();i++)
        {
            outfile << "," << Vector_action[i];
        }
        outfile << "\n";
    }

    if(sequence_num>0)
    {
        outfile << "\t\t" << "component condition " << Vector_sequence[0][0];
        for(int i =1;i< Vector_sequence.size();i++)
        {
            outfile << "," << Vector_sequence[i][0];
        }
        outfile << "\n";
    }

    if(fallback_num>0)
    {
        outfile << "\t\t" << "component condition " << Vector_fallback[0][0];
        for(int i =1;i< fallback_num;i++)
        {
            outfile << "," << Vector_fallback[i][0];
        }
        outfile << "\n";
    }

    if(sequence_num>0)
    {
        outfile << "\n";
        for(int i =0;i< Vector_sequence.size();i++)
        {
            for(int j = 1;j< Vector_sequence[i].size();j++)
            {
                outfile << "\t\t" << "connector rendezvous " << Vector_sequence[i][0] << "_to_" << Vector_sequence[i][j] << "_tick("  << Vector_sequence[i][0] << ".tick"<< j << "," << Vector_sequence[i][j] << ".tick)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_sequence[i][0] << "_to_" << Vector_sequence[i][j] << "_success("  << Vector_sequence[i][0] << ".success" << j << "," << Vector_sequence[i][j] << ".success)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_sequence[i][0] << "_to_" << Vector_sequence[i][j] << "_failure("  << Vector_sequence[i][0] << ".failure" << j << "," << Vector_sequence[i][j] << ".failure)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_sequence[i][0] << "_to_" << Vector_sequence[i][j] << "_running("  << Vector_sequence[i][0] << ".running" << j << "," << Vector_sequence[i][j] << ".running)" <<"\n";
                
            }
        }
    }

    if(fallback_num>0)
    {
        outfile << "\n";
        for(int i =0;i< Vector_fallback.size();i++)
        {
            for(int j = 1;j< Vector_fallback[i].size();j++)
            {
                outfile << "\t\t" << "connector rendezvous " << Vector_fallback[i][0] << "_to_" << Vector_fallback[i][j] << "_tick("  << Vector_fallback[i][0] << ".tick"<< j << "," << Vector_fallback[i][j] << ".tick)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_fallback[i][0] << "_to_" << Vector_fallback[i][j] << "_success("  << Vector_fallback[i][0] << ".success" << j << "," << Vector_fallback[i][j] << ".success)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_fallback[i][0] << "_to_" << Vector_fallback[i][j] << "_failure("  << Vector_fallback[i][0] << ".failure" << j << "," << Vector_fallback[i][j] << ".failure)" <<"\n";
                outfile << "\t\t" << "connector rendezvous " << Vector_fallback[i][0] << "_to_" << Vector_fallback[i][j] << "_running("  << Vector_fallback[i][0] << ".running" << j << "," << Vector_fallback[i][j] << ".running)" <<"\n";
                
            }
        }
    }
    outfile << "\tend\n";
    outfile << "\tcomponent system sys\n";
    outfile << "end\n";

    outfile.close();
}

void GetEleValue_for_name(tinyxml2::XMLElement * element)
{
    //Vector_condition,Vector_action,Vector_sequence,Vector_fallback
    for (tinyxml2::XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
	{
	    tree_num ++;
        tinyxml2::XMLElement* tmpele = currentele;

        const int children_count = ChildrenCount(tmpele);
        const char* name = tmpele->Name(); 

        if(!strcmp(tmpele->Name(),"Condition"))
        {
            Vector_condition.push_back(tmpele->Attribute( "name" ));
            // conditionPrint(tmpele->Attribute( "name" ),tree_num);
        }

        if(!strcmp(tmpele->Name(),"Action"))
        {
            Vector_action.push_back(tmpele->Attribute( "name" ));
            // actionPrint(tmpele->Attribute( "name" ),tree_num);
        }

        if(!strcmp(tmpele->Name(),"Fallback"))
        {
            std::vector<std::string> numFall;
            numFall.push_back(tmpele->Attribute( "name" ));
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numFall.push_back(node->Attribute( "name" ));
            }
            Vector_fallback.push_back(numFall);
            // std::cout << std::endl;
        }

        if(!strcmp(tmpele->Name(),"Sequence"))
        {
            std::vector<std::string> numSeq;
            numSeq.push_back(tmpele->Attribute( "name" ));
            for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
            {
                numSeq.push_back(node->Attribute( "name" ));
            }
            Vector_sequence.push_back(numSeq);
            // std::cout << std::endl;
        }
        
	    if (!tmpele->NoChildren())
		    GetEleValue_for_name(tmpele);
	}
}

// void GetEleValue(tinyxml2::XMLElement * element)
// {

//     for (tinyxml2::XMLElement* currentele = element->FirstChildElement(); currentele; currentele = currentele->NextSiblingElement())
// 	{
// 	    tree_num ++;
//         tinyxml2::XMLElement* tmpele = currentele;

//         const int children_count = ChildrenCount(tmpele);
//         const char* name = tmpele->Name();

//         if(!strcmp(name,"Sequence") || !strcmp(name,"Fallback"))
//         {
//             std::cout << "I " << tmpele->Name() << " have " <<subChildrenCount(tmpele) << " nums sub tree!  and sub chaild have " 
//                                                                                        << subChildHaveComposite(tmpele) << " nums Composite"<< std::endl;
//         }        

//         if(!strcmp(tmpele->Name(),"Condition"))
//         {
//             // conditionPrint(tmpele->Attribute( "name" ),tree_num);
//         }

//         if(!strcmp(tmpele->Name(),"Action"))
//         {
//             // actionPrint(tmpele->Attribute( "name" ),tree_num);
//         }

//         if(!strcmp(tmpele->Name(),"Fallback"))
//         {
//             std::vector<std::string> numSeq;
//             for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
//             {
//                 numSeq.push_back(node->Attribute( "name" ));
//             }
//             std::cout << std::endl;
//             // fallbackPrint(tmpele->Attribute( "name" ),numSeq,tree_num,tmpele);
//         }

//         if(!strcmp(tmpele->Name(),"Sequence"))
//         {
//             std::vector<std::string> numSeq;
//             for (auto node = tmpele->FirstChildElement(); node != nullptr;node = node->NextSiblingElement())
//             {
//                 numSeq.push_back(node->Attribute( "name" ));
//             }
//             std::cout << std::endl;
//             // sequencePrint(tmpele->Attribute( "name" ),numSeq,tree_num,tmpele);
//         }
        
// 	    if (!tmpele->NoChildren())
// 		    GetEleValue(tmpele);
// 	}
// }

std::string CharToStr(char * contentChar)
{
	std::string tempStr;
	for (int i=0;contentChar[i]!='\0';i++)
	{
		tempStr+=contentChar[i];
	}
	return tempStr;
}


void ModifyLineData(char* fileName, int lineNum)
{
	std::ifstream in;
	in.open(fileName);
 
	std::string strFileData = "";
	int line = 1;
	char tmpLineData[1024] = {0};
	while(in.getline(tmpLineData, sizeof(tmpLineData)))
	{
		if (line == lineNum)
		{
			strFileData += "MODULE main \n";
		}
        else
		{
			strFileData += CharToStr(tmpLineData);
			strFileData += "\n";
        }

		line++;
	}
	in.close();
 
	//写入文件
	std::ofstream out;
	out.open(fileName);
	out.flush();
	out<<strFileData;
	out.close();
}

void ModifyLineData_main(char* fileName, int lineNum ,int root_sub_num)
{
	std::ifstream in;
	in.open(fileName);
    // std::cout << "lineNum +++++++++++++++++++++++++++++++++" << lineNum << std::endl;
 
	std::string strFileData = "\ttick:{1";
    std::string str_FileData = "";
    for(int i = 2; i <= root_sub_num;i++)
    {
        strFileData = strFileData + "," + std::to_string(i);;
    }
    strFileData =strFileData + "};\n";

	int line = 0;
	char tmpLineData[1024] = {0};
	while(in.getline(tmpLineData, sizeof(tmpLineData)))
	{
		line ++;
        if (lineNum == line)
		{
			str_FileData += strFileData;
		}
        else
		{
			str_FileData += CharToStr(tmpLineData);
			str_FileData += "\n";
        }

	}
	in.close();
 
	//写入文件
	std::ofstream out;
	out.open(fileName);
	out.flush();
	out<<str_FileData;
	out.close();
}


int main( int argc, char* argv[] )
{
	
    tinyxml2::XMLDocument doc;
	if ( doc.LoadFile("../mytree.xml") )
	{
		doc.PrintError();
		exit( 1 );
	}
 
	// 根元素
    
	tinyxml2::XMLElement* xml_root = doc.RootElement();
    int all_sub_num = -1;
    int root_sub_num = allsubChildrenCount(xml_root,all_sub_num);
    std::cout << "root_sub_num is " << root_sub_num << std::endl;
    
    if (!xml_root || !StrEqual(xml_root->Name(), "root"))
    {
        throw std::runtime_error("The XML must have a root node called <root>");
    }
    std::ofstream outfile("../mytree.bip");

    node_num(xml_root);
    GetEleValue_for_name(xml_root);
    std::cout << "Vector_sequence.size() is " << Vector_sequence.size() << std::endl;
    std::cout << "sub Vector_sequence.size() is " << Vector_sequence[1].size() << std::endl;

    main_Model_Print();

    
    

    // ModifyLineData("../mytree.bip",1);
    // ModifyLineData_main("../mytree.bip",3,root_sub_num);
    // ModifyLineData("../mytree.bip",1);

    return 0;
}


// CTLSPEC
    // E[((fallback_node.status = success)->(action_init.status = success)) 
    // & ((closeup_fallback_node.status = success) -> (asmb_fallback_node.status = success))
    // & ((k-c_sequence_node.status = success) -> (closeup_fallback_node.status = success))
    // & ((fd_fallback_node.status = success) -> (closeup_fallback_node.status = success))
    // & ((fx_fallback_node.status = success) -> (fd_fallback_node.status = success))
    // & ((tra_fallback_node.status = success) -> (fx_fallback_node.status = success))
    // & ((tar_fallback_node.status = success) -> (tra_fallback_node.status = success))
    // & ((eng_fallback_node.status = success) -> (tar_fallback_node.status = success))
    // & ((eng_ass_fallback_node.status = success) -> (eng_fallback_node.status = success))
    // & ((end_fallback_node.status = success) -> (eng_ass_fallback_node.status = success))]

    // CTLSPEC
    // !E[((fallback_node.status = success)->(action_init.status = success)) U ((fd_fallback_node.status = success) -> (closeup_fallback_node.status = success))]
    // -- !E[((s1.status = success)->(s35.status = success)) U ((s13.status = success)->(s9.status = success))]

// SPEC !E[(s1.status = success)->(s35.status = success)U(s1.status = success & s35.status = success)]

// SPEC !E[(s2.status = success)->(s1.status = success)U(s1.status = success & s2.status = success)]
// SPEC !E[(s2.s6.s13.s14.status = success)->(s2.s6.s10.status = success)U(s2.s6.s13.s14.status = success & s2.s6.s10.status = success)]

// LTLSPEC G(!((s2.s3.status = success) & (s2.s4.status = failure) & (s5.s7.status = failure)))
// LTLSPEC G(!((s5.s6.status = success) & (s5.s7.status = success) & (s2.s4.status = failure)))