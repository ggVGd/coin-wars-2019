#pragma once
#include <stack>
#include <unordered_map>

namespace Cinnabar
{
	class PostfixEvaluator
	{
	public:
		void setPrecision(int);
		int precision() const;

		void set(const std::string&, const std::string&);
		void set(const std::string&, const float&);
		const std::string& get(const std::string&) const;
		float getf(const std::string&) const;

		void setConst(const std::string&, const std::string&);
		void setConst(const std::string&, const float&);

		std::string evaluate(const char*);
		float evaluatef(const char*);

	private:
		int _precision = 6;
		std::unordered_map<std::string, std::string> _constants;
		std::unordered_map<std::string, std::string> _variables;

		typedef std::stack<std::string> TokenStack;
		typedef void (PostfixEvaluator::*Function)(TokenStack&);

		void _push(TokenStack&, const char*);
		void _push(TokenStack&, const std::string&);
		void _push(TokenStack&, const float&);
		std::string _pop(TokenStack&);
		float _popf(TokenStack&);
		bool _popb(TokenStack&);

		static const std::unordered_map<std::string, Function> _functions;

		void _set(TokenStack&);
		void _unset(TokenStack&);

		void _add(TokenStack&);
		void _subtract(TokenStack&);
		void _multiply(TokenStack&);
		void _divide(TokenStack&);
		void _modulo(TokenStack&);
		void _power(TokenStack&);
		void _sqrt(TokenStack&);
		void _root(TokenStack&);

		void _sin(TokenStack&);
		void _cos(TokenStack&);
		void _tan(TokenStack&);

		void _round(TokenStack&);
		void _floor(TokenStack&);
		void _ceil(TokenStack&);

		void _if(TokenStack&);
		void _and(TokenStack&);
		void _or(TokenStack&);
		void _xor(TokenStack&);
		void _gt(TokenStack&);
		void _gte(TokenStack&);
		void _lt(TokenStack&);
		void _lte(TokenStack&);

		// Equality is provided, but really should not be relied on because all
		// math is using floating-point.
		void _eq(TokenStack&);
		void _neq(TokenStack&);

	};
}
