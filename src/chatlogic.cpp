#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////

    // create instance of chatbot
  // std::cout<<"ChatLogic constructor.  \n";
  //  _chatBot =  new ChatBot("../images/chatbot.png");

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    //_chatBot->SetChatLogicHandle(this);

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE
    ////
   
    // delete chatbot instance
  //std::cout<<"ChatLogic destructor. This deletes a chatbot \n";
  //  delete _chatBot;

    // delete all nodes
   /*HM// for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        delete *it;
    }//HM*/
  /*
    // delete all edges
    for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    {
        delete *it;
    }
*/
    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            ChatLogic::tokenlist::iterator type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            //itt felette a find_if 3. parametere egy bool-ra konvertálhato kifejezes, az elotte levo [] egy iterator, a soron levo pair cimet adja be a return reszbe, es megnezi, hogy annak az elso tagja "TYPE"-e)
            if (type != tokens.end())
            {
                // check for id // az auto itt is ChatLogic::tokenlist::iterator -t jelent. Gondolom, ez a tokenlist soron lévő eleme, ami egy 2 string-ből álló pair
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE
                        ////

                        // check if node with this ID exists already

                        /* original code parameter passing: Lesson 5 - Transferring ownership:With the non-owning raw pointer * or the reference & we can observe an 
                        object from which we can assume that its lifetime will exceed the lifetime of the function parameter. 
                        In concurrency however, this might not be the case, but for linear code we can safely assume this. 
                        To decide wether a * or & is more appropriate, you should think about wether you need to express that there is no object. 
                        This can only be done with pointers by passing e.g. nullptr. In most other cases, you should use a reference instead.*/
                        //HM//auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](GraphNode *node) 

                        //here below I replaced auto for the sake of better understanding of the code - for me.
                        //Task 3 description: "When passing the GraphNode instances to functions, make sure to not transfer ownership and try to contain the changes to class ChatLogic where possible."
                        //Lesson 5 - Transferring ownership: void f (unique_ptr<object>) - the object sink: this one passes the ownership : "After the object has been passed in this way, the caller will have an invalid unique pointer and the function to which the object now belongs may destroy it or move it somewhere else."
                        //void f (unique_ptr<object>&) - In and Out Again 1: this one does not pass ownership: "In some cases, we want to modify a unique pointer (not necessarily the underlying object) and re-use it in the context of the caller. In this case, method d) from the above list might be most suitable"
                        //That's the reason why & is used in the parameter passing below:
                        std::vector<std::unique_ptr<GraphNode>>::iterator  newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode> &node)                      
                        
                       { //std::cout<<"get nelkul: "<<node->GetID()<<"\n";
                         //std::cout<<"get -tel:: "<<node.get()->GetID()<<"\n";
                            return node->GetID() == id; }); //HM Task3////HM: nem kell .get() ? miért? ugyanazt kapom mindkettovel, kiirattam cout-tal. Lehet, hogy mindkettő ugyanoda mutat, és azért.

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            _nodes.emplace_back(new GraphNode(id));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            ////HM Task3///auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](GraphNode *node) { return node->GetID() == std::stoi(parentToken->second); });
                            ////HM Task3///auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](GraphNode *node) { return node->GetID() == std::stoi(childToken->second); });


                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode> &node) { return  node->GetID() == std::stoi(parentToken->second); });//HM Task3//
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode> &node)   { return node->GetID() == std::stoi(childToken ->second); });//HM Task3//
                            // create new edge
                            //HM Task4  GraphEdge *edge = new GraphEdge(id);
                            std::unique_ptr<GraphEdge> edge=std::make_unique<GraphEdge>(id); //a konstruktor var egy int-et
                            //HM Task3//edge->SetChildNode(*childNode);
                            //HM Task3// edge->SetParentNode(*parentNode);

                            edge->SetChildNode((*childNode).get()); //HM Task3//
                            edge->SetParentNode((*parentNode).get()); //HM Task3//
                            _edges.push_back(edge.get());

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            //HM Task4 (*childNode)->AddEdgeToParentNode(edge);
                            //HM Task4 (*parentNode)->AddEdgeToChildNode(edge);
                            (*childNode)->AddEdgeToParentNode(edge.get()); //HM Task4 not owned, ez sima pointert var, ezert kell a .get()
                            (*parentNode)->AddEdgeToChildNode(std::move(edge)); //HM Task4 unique pointert csak move-olással lehet atadni, mert nem lehet masolni Lesson 5 Transferring ownership Guidelines p2 video
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
              //HM Task3// rootNode = *it;
                rootNode = (*it).get(); // assign current node to root //HM Task3//
                //std::cout<<"chatlogic.cpp line 240 LoadAnswerGraphFromFile - rootnode assigned \n";
  
            }
            else
            {
               // std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // add chatbot to graph root node
    
  ChatBot localChatbot("../images/chatbot.png");
 // std::cout<<"chatlogic.cpp line 252 LoadAnswerGraphFromFile - localchatbot constuctor \n";
  
 //std::cout<<"chatlogic.cpp line 255 LoadAnswerGraphFromFile -SetChatLogicHandle \n";
  SetChatbotHandle(&localChatbot);
  localChatbot.SetChatLogicHandle(this);
  //std::cout<<"chatbot.cpp line 257 LoadAnswerGraphFromFile - setchatbothandle \n";
  localChatbot.SetRootNode(rootNode);
 // std::cout<<"chatlogic.cpp line 259 LoadAnswerGraphFromFile - setrootnode \n";
  rootNode->MoveChatbotHere(std::move(localChatbot)); //HM Task5 https://knowledge.udacity.com/questions/131653
 // std::cout<<"chatlogic.cpp line 261 LoadAnswerGraphFromFile - movechatbothere \n";
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    //std::cout<<"chatlogic.cpp line 273 LoadAnswerGraphFromFile -  \n";
    _chatBot = chatbot;
    //std::cout<<"chatlogic.cpp line 275 LoadAnswerGraphFromFile - chatbot assigned \n";
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    //std::cout<<"chatlogic.cpp line 285 SendMessageToUser -  \n";
    _panelDialog->PrintChatbotResponse(message);
   // std::cout<<"chatlogic.cpp line 287 SendMessageToUser - _panelDialog->PrintChatbotResponsed \n";
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
