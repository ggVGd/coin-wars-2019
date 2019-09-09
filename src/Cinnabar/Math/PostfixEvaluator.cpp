#include "PostfixEvaluator.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <queue>

namespace Cinnabar
{
	void PostfixEvaluator::setPrecision(int precision)
	{
		_precision = precision;
	}
	int PostfixEvaluator::precision() const
	{
		return _precision;
	}
	void PostfixEvaluator::set(const std::string& key, const std::string& s)
	{
		_variables[key] = s;
	}
	void PostfixEvaluator::set(const std::string& key, const float& f)
	{
		char buf[32];
		sprintf(buf, "%.*f", _precision, f);
		set(key, buf);
	}
	const std::string& PostfixEvaluator::get(const std::string& key) const
	{
		return _variables.at(key);
	}
	float PostfixEvaluator::getf(const std::string& key) const
	{
		return strtof(get(key).c_str(), nullptr);
	}
	void PostfixEvaluator::setConst(const std::string& key, const std::string& s)
	{
		_constants[key] = s;
	}
	void PostfixEvaluator::setConst(const std::string& key, const float& f)
	{
		char buf[32];
		sprintf(buf, "%.*f", _precision, f);
		setConst(key, buf);
	}
	std::string PostfixEvaluator::evaluate(const char* input)
	{
		std::queue<const char*> tokens;

		const std::size_t len = strlen(input);
		char* buf = new char[len + 1];
		memcpy(buf, input, len);
		buf[len] = 0;

		static const char* delimiters = " \t\r\n";
		char* pch = strtok(buf, delimiters);
		while(pch != nullptr)
		{
			tokens.push(pch);
			pch = strtok(nullptr, delimiters);
		}

		TokenStack stack;
		while(!tokens.empty())
		{
			const char* token = tokens.front();
			tokens.pop();

			auto it = _functions.find(token);
			if(it != _functions.end())
			{
				(this->*it->second)(stack);
			}
			else
			{
				stack.push(token);
			}
		}

		delete[] buf;

		return _pop(stack);
	}
	float PostfixEvaluator::evaluatef(const char* input)
	{
		const std::string r = evaluate(input);
		return strtof(r.c_str(), nullptr);
	}
	void PostfixEvaluator::_push(TokenStack& stack, const char* s)
	{
		stack.push(s);
	}
	void PostfixEvaluator::_push(TokenStack& stack, const std::string& s)
	{
		stack.push(s.c_str());
	}
	void PostfixEvaluator::_push(TokenStack& stack, const float& f)
	{
		char buf[32];
		sprintf(buf, "%.*f", _precision, f);
		stack.push(buf);
	}
	std::string PostfixEvaluator::_pop(TokenStack& stack)
	{
		const auto t = stack.top();
		stack.pop();

		auto it = _constants.find(t);
		if(it != _constants.end())
		{
			return it->second;
		}

		it = _variables.find(t);
		if(it != _variables.end())
		{
			return it->second;
		}

		return t;
	}
	float PostfixEvaluator::_popf(TokenStack& stack)
	{
		const auto t = _pop(stack);
		return strtof(t.c_str(), nullptr);
	}
	bool PostfixEvaluator::_popb(TokenStack& stack)
	{
		const float f = _popf(stack);
		return f != 0.0f;
	}


	const std::unordered_map<std::string, PostfixEvaluator::Function> PostfixEvaluator::_functions = {
		{ "set",   &PostfixEvaluator::_set      },
		{ "unset", &PostfixEvaluator::_unset    },
		{ "+",     &PostfixEvaluator::_add      },
		{ "-",     &PostfixEvaluator::_subtract },
		{ "*",     &PostfixEvaluator::_multiply },
		{ "/",     &PostfixEvaluator::_divide   },
		{ "%",     &PostfixEvaluator::_modulo   },
		{ "/",     &PostfixEvaluator::_divide   },
		{ "^",     &PostfixEvaluator::_power    },
		{ "sqrt",  &PostfixEvaluator::_sqrt     },
		{ "root",  &PostfixEvaluator::_root     },
		{ "sin",   &PostfixEvaluator::_sin      },
		{ "cos",   &PostfixEvaluator::_cos      },
		{ "tan",   &PostfixEvaluator::_tan      },
		{ "round", &PostfixEvaluator::_round    },
		{ "floor", &PostfixEvaluator::_floor    },
		{ "ceil",  &PostfixEvaluator::_ceil     },
		{ "if",    &PostfixEvaluator::_if       },
		{ "and",   &PostfixEvaluator::_and      },
		{ "or",    &PostfixEvaluator::_or       },
		{ "xor",   &PostfixEvaluator::_xor      },
		{ ">",     &PostfixEvaluator::_gt       },
		{ ">=",    &PostfixEvaluator::_gte      },
		{ "<",     &PostfixEvaluator::_lt       },
		{ "<=",    &PostfixEvaluator::_lte      },
		{ "==",    &PostfixEvaluator::_eq       },
		{ "!=",    &PostfixEvaluator::_neq      },
	};
	void PostfixEvaluator::_set(TokenStack& stack)
	{
		const std::string s2 = _pop(stack);
		const std::string s1 = _pop(stack);
		_variables[s1] = s2;
	}
	void PostfixEvaluator::_unset(TokenStack& stack)
	{
		const std::string s = _pop(stack);
		_variables.erase(s);
	}
	void PostfixEvaluator::_add(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 + f2);
	}
	void PostfixEvaluator::_subtract(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 - f2);
	}
	void PostfixEvaluator::_multiply(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 * f2);
	}
	void PostfixEvaluator::_divide(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 / f2);
	}
	void PostfixEvaluator::_modulo(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, fmod(f1, f2));
	}
	void PostfixEvaluator::_power(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, pow(f1, f2));
	}
	void PostfixEvaluator::_sqrt(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, sqrt(f));
	}
	void PostfixEvaluator::_root(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, pow(f1, 1.0f / f2));
	}
	void PostfixEvaluator::_sin(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, sin(f));
	}
	void PostfixEvaluator::_cos(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, cos(f));
	}
	void PostfixEvaluator::_tan(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, tan(f));
	}
	void PostfixEvaluator::_round(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, round(f));
	}
	void PostfixEvaluator::_floor(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, floor(f));
	}
	void PostfixEvaluator::_ceil(TokenStack& stack)
	{
		const float f = _popf(stack);
		_push(stack, ceil(f));
	}
	void PostfixEvaluator::_if(TokenStack& stack)
	{
		const std::string s3 = _pop(stack);
		const std::string s2 = _pop(stack);
		const float b1 = _popb(stack);
		_push(stack, b1 ? s2 : s3);
	}
	void PostfixEvaluator::_and(TokenStack& stack)
	{
		const bool b2 = _popb(stack);
		const bool b1 = _popb(stack);
		_push(stack, b1 && b2);
	}
	void PostfixEvaluator::_or(TokenStack& stack)
	{
		const bool b2 = _popb(stack);
		const bool b1 = _popb(stack);
		_push(stack, b1 || b2);
	}
	void PostfixEvaluator::_xor(TokenStack& stack)
	{
		const bool b2 = _popb(stack);
		const bool b1 = _popb(stack);
		_push(stack, b1 ^ b2);
	}
	void PostfixEvaluator::_gt(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 > f2);
	}
	void PostfixEvaluator::_gte(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 >= f2);
	}
	void PostfixEvaluator::_lt(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 < f2);
	}
	void PostfixEvaluator::_lte(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 <= f2);
	}
	void PostfixEvaluator::_eq(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 == f2);
	}
	void PostfixEvaluator::_neq(TokenStack& stack)
	{
		const float f2 = _popf(stack);
		const float f1 = _popf(stack);
		_push(stack, f1 != f2);
	}
}
