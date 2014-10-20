// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// eoEasyEA.h
// (c) GeNeura Team, 1998
/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
 */
//-----------------------------------------------------------------------------

#ifndef _pti_eoEasyEA_h
#define _pti_eoEasyEA_h

//-----------------------------------------------------------------------------

//UCI Documentation:
//-----------------------------------------------------------------------------
/* Kris Carlson 10/31/2011
   This class inherits from pti_eoAlgo.h just as eoEasyEA inherits from 
   eoAlgo.  It provides the appropriate function definitions to implement the 
   separate steps of a generic evolutionary algorithm in parts rather than all 
   in one large step.  These function definitions were taken from the () 
   operator in the original eoEasyEA.h header file.
*/
//-----------------------------------------------------------------------------

#include <apply.h>
#include <eoAlgo.h>
//#include <eoPopAlgo.h> //deprecated: does not exist anymore
#include <eoPopEvalFunc.h>
#include <eoContinue.h>
#include <eoSelect.h>
#include <eoTransform.h>
#include <eoBreed.h>
#include <eoMergeReduce.h>
#include <eoReplacement.h>
#include "pti_eoAlgo.h"

#include <cstdio>

template <class EOT> class eoIslandsEasyEA ;

template <class EOT> class eoDistEvalEasyEA ;

/** An easy-to-use evolutionary algorithm; you can use any chromosome,
    and any selection transformation, merging and evaluation
    algorithms; you can even change in runtime parameters of those
    sub-algorithms

Change (MS, July 3. 2001):
  Replaced the eoEvalFunc by an eoPopEvalFunc: this immediately
  allows many useful constructs, such as co-evolution (e.g. game players),
  parisian approach (the solution to the problem is the whole population)
  or simple distribution of evaluations on a cluster.
  In case an eoEvalFunc is passed, it is embedded on an eoPopLoopEval
  This makes things a little uglier (required an additional "dummy" member

Note: it looks ugly only because we wanted to authorize many different
  constructors. Please only look at the operator() and there shall be light
*/
template<class EOT> class pti_eoEasyEA: public eoModlibAlgo<EOT> {
public:

  /** Ctor taking a breed and merge */
     pti_eoEasyEA(
         eoContinue<EOT>& _continuator,
         eoEvalFunc<EOT>& _eval,
         eoBreed<EOT>& _breed,
         eoReplacement<EOT>& _replace
     ) : continuator(_continuator),
	 eval (_eval),
	 loopEval(_eval),
	 popEval(loopEval),
	 selectTransform(dummySelect, dummyTransform),
         breed(_breed),
         mergeReduce(dummyMerge, dummyReduce),
         replace(_replace)
         {}

  /*
  pti_eoEasyEA(eoContinue <EOT> & _continuator,
	   eoPopEvalFunc <EOT> & _pop_eval,
	   eoBreed <EOT> & _breed,
	   eoReplacement <EOT> & _replace
	   ) :
    continuator (_continuator),
    eval (dummyEval),
    loopEval(dummyEval),
    popEval (_pop_eval),
    selectTransform (dummySelect, dummyTransform),
    breed (_breed),
    mergeReduce (dummyMerge, dummyReduce),
    replace (_replace) {

  }
  */

  /** NEW Ctor taking a breed and merge and an eoPopEval */
     pti_eoEasyEA(
         eoContinue<EOT>& _continuator,
         eoPopEvalFunc<EOT>& _eval,
         eoBreed<EOT>& _breed,
         eoReplacement<EOT>& _replace
     ) : continuator(_continuator),
	 eval (dummyEval),
	 loopEval(dummyEval),
         popEval(_eval),
         selectTransform(dummySelect, dummyTransform),
         breed(_breed),
         mergeReduce(dummyMerge, dummyReduce),
         replace(_replace)
         {}

  /// Ctor eoBreed, eoMerge and eoReduce.
    pti_eoEasyEA(
         eoContinue<EOT>& _continuator,
         eoEvalFunc<EOT>& _eval,
         eoBreed<EOT>& _breed,
         eoMerge<EOT>& _merge,
         eoReduce<EOT>& _reduce
     ) : continuator(_continuator),
	 eval (_eval),
	 loopEval(_eval),
	 popEval(loopEval),
         selectTransform(dummySelect, dummyTransform),
         breed(_breed),
         mergeReduce(_merge, _reduce),
         replace(mergeReduce)
         {}

  /// Ctor eoSelect, eoTransform, and eoReplacement
    pti_eoEasyEA(
         eoContinue<EOT>& _continuator,
         eoEvalFunc<EOT>& _eval,
         eoSelect<EOT>& _select,
         eoTransform<EOT>& _transform,
         eoReplacement<EOT>& _replace
     ) : continuator(_continuator),
	 eval (_eval),
	 loopEval(_eval),
	 popEval(loopEval),
         selectTransform(_select, _transform),
         breed(selectTransform),
         mergeReduce(dummyMerge, dummyReduce),
         replace(_replace)
         {}

    /// Ctor eoSelect, eoTransform, eoMerge and eoReduce.
     pti_eoEasyEA(
         eoContinue<EOT>& _continuator,
         eoEvalFunc<EOT>& _eval,
         eoSelect<EOT>& _select,
         eoTransform<EOT>& _transform,
         eoMerge<EOT>&     _merge,
         eoReduce<EOT>&    _reduce
     ) : continuator(_continuator),
	 eval (_eval),
	 loopEval(_eval),
	 popEval(loopEval),
         selectTransform(_select, _transform),
         breed(selectTransform),
         mergeReduce(_merge, _reduce),
         replace(mergeReduce)
         {}

  virtual void getOffspring (eoPop<EOT>& _pop, eoPop<EOT>& _offspring)
  {
    //fprintf(stderr, "getting a new offspring from the population\n");
    //printf("getting a new offspring from the population ...\n");
    _offspring.clear(); // new offspring
    breed(_pop, _offspring);
  }
  
  virtual bool evaluatePopulation (eoPop<EOT>& _pop)
  {
    eoPop<EOT> empty_pop;
    popEval(empty_pop, _pop); //A first eval of pop.
    return true; //Always continue after first evaluation
  }
  
  virtual bool evaluateAndReplacePopulation (eoPop<EOT>& _pop, eoPop<EOT>& _offspring)
  {
    //fprintf(stderr, "Evaluating the offspring and creating a new parent for the population..\n");
    //printf("evaluating the offspring and creating a new parent for the population ...\n");
    try
      {
	unsigned pSize = _pop.size();
	popEval(_pop, _offspring); // eval of parents + offspring if necessary
	replace(_pop, _offspring); // after replace, the new pop. is in _pop
	
	if (pSize > _pop.size())
	  throw std::runtime_error("Population shrinking!");
	else if (pSize < _pop.size())
	  throw std::runtime_error("Population growing!");
      }
    catch (std::exception& e)
      {
	std::string s = e.what();
	s.append( " in eoEasyEA");
	throw std::runtime_error( s );
      }

    bool continueEvolution = continuator( _pop );
    return continueEvolution;
  }
  
  /// Apply a few generation of evolution to the population.
  virtual void operator()(eoPop<EOT>& _pop)
  {
    eoPop<EOT> offspring, empty_pop;
    popEval(empty_pop, _pop); // A first eval of pop.
    do
    {
      try
      {
         unsigned pSize = _pop.size();
         offspring.clear(); // new offspring

         breed(_pop, offspring);

         popEval(_pop, offspring); // eval of parents + offspring if necessary

         replace(_pop, offspring); // after replace, the new pop. is in _pop

         if (pSize > _pop.size())
             throw std::runtime_error("Population shrinking!");
         else if (pSize < _pop.size())
             throw std::runtime_error("Population growing!");

      }
      catch (std::exception& e)
      {
	    std::string s = e.what();
	    s.append( " in eoEasyEA");
	    throw std::runtime_error( s );
      }
    } while ( continuator( _pop ) );
  }

protected :

  // If selectTransform needs not be used, dummySelect and dummyTransform are used
  // to instantiate it.
  class eoDummySelect : public eoSelect<EOT>
     { public : void operator()(const eoPop<EOT>&, eoPop<EOT>&) {} } dummySelect;

     class eoDummyTransform : public eoTransform<EOT>
     { public : void operator()(eoPop<EOT>&) {} } dummyTransform;

  class eoDummyEval : public eoEvalFunc<EOT>
  {public: void operator()(EOT &) {} } dummyEval;

  eoContinue<EOT>&          continuator;

  eoEvalFunc <EOT> &        eval ;
  eoPopLoopEval<EOT>        loopEval;

  eoPopEvalFunc<EOT>&       popEval;

  eoSelectTransform<EOT>    selectTransform;
  eoBreed<EOT>&             breed;

  // If mergeReduce needs not be used, dummyMerge and dummyReduce are used
  // to instantiate it.
  eoNoElitism<EOT>          dummyMerge;
  eoTruncate<EOT>           dummyReduce;

  eoMergeReduce<EOT>        mergeReduce;
  eoReplacement<EOT>&       replace;

  // Friend classes
  friend class eoIslandsEasyEA <EOT> ;
  friend class eoDistEvalEasyEA <EOT> ;
};

//-----------------------------------------------------------------------------

#endif

