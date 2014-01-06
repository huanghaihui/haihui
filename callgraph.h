#ifndef CALLGRAPH_H
#define CALLGRAPH_H
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/IncludeFile.h"
#include "llvm/Support/ValueHandle.h"
#include <map>

using namespace llvm;

namespace callgraph {

class Function;
class Module;
class CallGraphNode;

class CallGraph
{
protected:
    Module * Mod; //the module this call garph represents

    typedef std::map<const Function *, CallGraphNode *> FunctionMapTy;
    FunctionMapTy FunctionMap;  //Map from a function to its node
public:
    static char ID; //class identification,replacement for typeifo

    //Accessors
    typedef FunctionMapTy::iterator iterator;
    typedef FunctionMapTy::const_iterator const_iterator;

    Module &getModule() const { return*Mod; }


    inline	iterator begin()	{ return FunctionMap.begin(); }
    inline	iterator end()		{ return FunctionMap.end();   }
    inline const_iterator begin() const { return FunctionMap.begin(); }
    inline const_iterator end()   const { return FunctionMap.end();   }

    //return the call garph node for the provided function
    inline const CallGraphNode *operator[](const Function * F) const{
    	const_iterator I = FunctionMap.find(F);
	assert(I != FunctionMap.end() && "Function not in callgraph");
	    return I->second;
    }

    inline CallGraphNode * operator[](const Function * F){
    	const_iterator I = FunctionMap.find(F);
	assert(I != FunctionMap.end() && "Function not in callgraph");
	return I->second;
    }

    //return callgraphnode which is used to represent the undetermined calls into the graph, override this if you want behavioral inheritance
    virtual CallGraphNode * getExternalCallingNode() const { return 0;}
    virtual CallGraphNode * getCallsExternalNode()   const { return 0;}
  
    //return the root/main method in the moduel ,or some other root node 
    virtual CallGraphNode* getRoot() { return 0; }
    virtual const CallGraphNode* getRoot() const { return 0; }


    Function * removeFunctionFromModule(CallGraphNode *CGN);
    Function * removeFunctionFromModule(Function* F){
       return removeFunctionFromModule((*this)[F]);
    }

    //is identical to calling operator[], but 
    //it will insert a new CallGraphNode for the special function if one dose not exist
   CallGraphNode * getOrInsertFunction(const Function * F);

   //..........................................................????????????
   void spliceFunction(const Function * From, const Function * To);


   //pass infrastructure interface glue code
   //
protected:
	CallGraph() {}  

	//initialize call this method before calling other methods
	//re/initializes the state of the CallGraph
	//
	void intialize(Module &M);

	void print(raw_ostream &o,Module *)const;
	void dump() const;
protected:
	//destroy release memory for the call graph
	virtual void destroy();
};	



class CallGraphNode{
    friend class CallGraph;

    AssertingVH<Function> F;

      //CallRecord this is a pair of the calling instruction(a call or invoke)
      //and the callgraph node being called
    public:
         typedef std::pair<WeakVH, CallGraphNode*> CallRecord;
    private:
	 std::vector<CallRecord> CalledFunctions;

      //NumReferences this is the number of times that this callgraphnode cocurs in the callgraphFunctions array of this or other callgraphnode
      unsigned NumReferences;

      CallGraphNode(const CallGraphNode &) ;//LLVM_DELETED_FUNCTION;
      void operator=(const CallGraphNode &);// LLVM_DELETED_FUNCTION;

      void DropRef()  { --NumReferences; }
      void AddRef()   { ++NumReferences; }

    public:
      typedef std::vector<CallRecord> CalledFunctionVector;

      //callgraphnode ctor -  create a node for the sepcifiled function
     /*
      inline CallGraphNode (Function f) : F(f) ,NumReferences(0) {}
      ~CallGraphNode(){
	  assert(NumReferences == 0 && "Node deleted while references remain");
      }

*/
      //Acceror methods
      //
      //

      typedef std::vector<CallRecord>::iterator iterator;
      typedef std::vector<CallRecord>::const_iterator const_iterator;

     //getFunction return the function that his call graph node represents
     //Function *getFunction() const { return F; } 

     inline iterator begin()  { return CalledFunctions.begin(); }
     inline iterator end()    { return CalledFunctions.end();   }
     inline const_iterator begin()  const { return CalledFunctions.begin(); }
     inline const_iterator end()    const { return CalledFunctions.end();   }
     inline bool empty() const    { return CalledFunctions.empty();  }
     inline unsigned size() const { return (unsigned)CalledFunctions.size(); }

     //getNumReferences return the number of other callgraphnode in this callgraph tha reference this node in their callee list
     unsigned getNumReferences() const { return NumReferences; }

     //subscripting operator - return the i'th called function
     //
     CallGraphNode *operator[] (unsigned i) const{
	 assert(i < CalledFunctions.size() && "Invalid index" );
	 return CalledFunctions[i].second;
     }


     //dump print out this call graph node
     //
     void dump() const;
     void print(raw_ostream & OS) const;



     //Methods to keep a call graph up to date with a function that has been modified
     void removeAllCallFunctions(){
	 while(!CalledFunctions.empty()){
	     CalledFunctions.back().second->DropRef();
	     CalledFunctions.pop_back();
	 }
     }
  




     void stealCalledFunctionsForm(CallGraphNode  * N){

	assert(CalledFunctions.empty()&&
       "cannot steal callsite information if I already have some");
	std::swap(CalledFunctions, N->CalledFunctions);	
     }



     void addCalledFunction(CallSite CS, CallGraphNode * M)
     {
	 assert(!CS.getInstruction()||
		 !CS.getCalledFunction() ||
		 !CS.getCalledFunction()->isIntrinsic());
	 CalledFunctions.push_back(std::make_pair(CS.getInstruction(),M));
	 M->AddRef();
     }

     void removeCallEdge(iterator I){
       I->second->DropRef();
       *I = CalledFunctions.back();
       CalledFunctions.pop_back();
     }



     //removeCallEdgeFor - this methond reomves the edge in the node for the specified call site .Note that this method takes linear time .so ,it shoule be used sparingly.
     void removeCallEdgeFor(CallSite CS);

     void removeAnyCallEdgeTo(CallGraphNode * Callee);

     void removeOneAbstractEdgetTo(CallGraphNode * Callee);

     void replaceCallEdge (CallSite CS,CallSite NewCS ,CallGraphNode *Callee);

     void addReferncesDropped(){
	 NumReferences = 0;
     }
};

/*
    class CallGraphPass : public FunctionPass{
	public:
	    static char ID;
	    CallgraphPass ();
	    virtual bool doInitalizatin(Module &);
	    virtual bool doFinalization(Module &);
	    virtual bool runOnFunction(Function &);
	    virtual void getAnalysisUsage(AnalysisUsage & ) const;
	    virtual ~CallgraphPass();
	 private:
	   CallGraph * buildCallgraph();
	   FILE * outfile; 
    }*/

}

#endif
