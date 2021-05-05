#pragma once
#include "MIArray.h"
namespace MI {
	//A template class allowing A logic predicate to be formed by objects, and two functions such that:
	//  1. applied on elements not being "notted" into the expression (!, ~),
	//			or otherwise being added normally (not via a method ending in Not)		
	//	2. applied for elements that are notted into the expression, typically returning a 
	//			positive result when the result of 1. is negative.
	// Note: this is meant to be linked into a parser. used something like:
	//		StartTrue(1); AndNew(11); OrNew(9); Evaluate(LessThan10, GreaterThan9); //returns true since || has precidence
	template <typename T, size_t MAX_SIZE>
	class LogicSequence {

		//MI::Array<int, MAX_SIZE> scopeBegins;
		//MI::Array<int, MAX_SIZE> scopeEnds;

		MI::Array<T, MAX_SIZE> m_iElms;
		MI::Array<bool, MAX_SIZE> trueMarkers; //true is true, false is false
		MI::Array<bool, MAX_SIZE - 1> orMarkers; //true is OR, false is AND
	public:
		LogicSequence() {};
		~LogicSequence() {};

		void Clear() {
			m_iElms.Clear();
			trueMarkers.Clear();
			orMarkers.Clear();
		}
		void StartTrue(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(true);
		}
		void StartFalse(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(false);
		}
		void OrNew(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(true);
			orMarkers.AddEnd(true);
		}
		void AndNew(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(true);
			orMarkers.AddEnd(false);
		}
		void OrNewNot(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(false);
			orMarkers.AddEnd(true);
		}
		void AndNewNot(const T& elm) {
			m_iElms.AddEnd(elm);
			trueMarkers.AddEnd(false);
			orMarkers.AddEnd(false);
		}

		//Algorithm: Evaluate n-th OR sequence, then evaluate n-th AND sequence, etc.
		template <typename Func>
		bool Test(Func TrueTest, Func FalseTest, int index) {
			if (trueMarkers[index]) {
				return TrueTest(m_iElms[index]);
			}
			else {
				return FalseTest(m_iElms[index]);
			}
		}

		//Algorithm: Evaluate n-th OR sequence, then evaluate n-th AND sequence, etc.
		template <typename Func>
		bool Evaluate(Func TrueTest, Func FalseTest) {
			for (int i = 0; i < orMarkers.Size(); i++) {
				if (!orMarkers[i]) {//and group
					//goto or group
					while (i < orMarkers.Size() && !orMarkers[i] && i < orMarkers.Size()) {
						if (!Test(TrueTest, FalseTest, i)) {
							return false;
						}
						i++;
					}
				}
				if (i < orMarkers.Size() && orMarkers[i]) { //or group
					while (i < orMarkers.Size() && !Test(TrueTest, FalseTest, i) && orMarkers[i]) {
						i++;
					}
					if (!orMarkers[i] && orMarkers[i - 1]) { //last or'd element
						if (!Test(TrueTest, FalseTest, i)) {
							return false;
						}
					}
					else { //or group is true
						do { i++; } while (i < orMarkers.Size() && orMarkers[i]);
					}
					i++;
				}
			}
			//When it gets here, it has passed all attempts at disproving, and one element
			//  remains, whether it's part of an & or an |, it decides the truth-value
			return Test(TrueTest, FalseTest, m_iElms.Size() - 1);
		}
	};

}