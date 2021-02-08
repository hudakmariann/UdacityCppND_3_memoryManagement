#include "graphedge.h"
#include "graphnode.h"
#include <iostream>//HM

GraphNode::GraphNode(int id)
{
    //std::cout<< "graphnode constructor \n";
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////
//std::cout<< "graphnode destructor \n";
  // delete _chatBot; //HM Task 0 


    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

//HM Task4 void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge)); //HM Task4
}
//// STUDENT CODE
////
//HM Task5 void GraphNode::MoveChatbotHere(ChatBot *chatbot)
//HM Task5 Knowledge https://knowledge.udacity.com/questions/117912 says: (ChatBot &&chatbot) - and this works. BUT:
//https://knowledge.udacity.com/questions/131653:  If you used the &&, ChatBot instance will be passed as a rvalue. Consequently, the move constructor will not be called as this message "ChatBot Move Constructor" will not be printed on the terminal, which is a requirement for Task 5.
void GraphNode::MoveChatbotHere(ChatBot chatbot) 

{
    _chatBot = std::move(chatbot);

  //std::cout<<"graphnode.cpp line 47 MoveChatbotHere - move(chatbot) \n";
    //HM Task5 knowledge _chatBot->SetCurrentNode(this);
    _chatBot.SetCurrentNode(this);
   // std::cout<<"graphnode.cpp line 50 MoveChatbotHere - setcurrentnode \n";
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot)); //HM Task5 https://knowledge.udacity.com/questions/131653
    //_chatBot = nullptr; // invalidate pointer at source
 // std::cout<<"graphnode.cpp line 54 MoveChatbotToNewNode - movechatbothere \n";
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    return _childEdges[index].get(); //HM Task4 .get()

    ////
    //// EOF STUDENT CODE
}