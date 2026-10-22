#include "simpleselectionpolicy.h"

using namespace mctsearch;

SimpleSelectionPolicy::SimpleSelectionPolicy(std::unique_ptr<ExpansionPolicy> e, std::unique_ptr<BestChild> b):
expansion(std::move(e)),
bestchild(std::move(b))
{

}

TreeSearchNode* SimpleSelectionPolicy::select_new(TreeSearchNode* node) const
 {
     TreeSearchNode* n = node;
     while (!n->terminal())
     {
         if (!n->is_full_expanded())
         {
             return expansion->expand(n);
         }
         else
         {
             n = bestchild->choose(n);
         }
     }
     return n;
 }