#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <cctype>
#include <string>
#include <algorithm>
#include <exception>
#include <map>
#include <cmath>


class ParseException : public std::exception {
protected:
	std::string msg_;
public:
	ParseException(std::string message) {
		this->msg_ = message;

	}
	virtual const char* what() const throw () {
		return msg_.c_str();
	}
};

class Bounds {
public:
	// lower <= x && x <= upper
	int upper, lower;
	Bounds()
	{
		this->upper = std::numeric_limits<int>::max();
		this->lower = std::numeric_limits<int>::min();
	}

	bool operator < (const Bounds & b) const {
		return false;
	}
};

class Variable {
public:
	int coefficient;
	size_t index;
	Variable(int coefficient, size_t index) :
		coefficient(coefficient), index(index)
	{
	}

	bool operator < (const Variable &b) const {
		return index < b.index;
	}
};

class Condition {
public:
	enum Type { eq, leq, geq };
	Type type;
	std::vector<Variable> variables;
	int constant;
	Condition(Type type, const std::vector<Variable> & variables, int constant):
		type(type), variables(variables), constant(constant)
	{
	}
};

class Data {
public:
	std::vector<Condition> conditions;
	std::vector< std::pair<size_t, Bounds> > bounds;
	std::vector<size_t> indices;
	std::vector<Variable> function;

	//static std::string Trim(const std::string &s);
	static std::vector<std::string> Split(const std::string & input, char delim);
	static std::string Join(const std::vector<std::string> & input);
	static std::vector<Variable> ParseVariables(const std::vector<std::string> & tokens, bool opposite = false);
	static size_t ParseVariable(std::string variable);
	static Condition ParseExpression(const std::vector<std::string> & tokens);
public:
	void Parse(const std::string & input);
	std::string Print();
};

//std::string Data::Trim(const std::string &s)
//{
//	auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
//	auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
//	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
//}

std::vector<std::string> Data::Split(const std::string & input, char delim)
{
	std::vector<std::string> result;
	std::stringstream buffer(input);
	for (std::string line; std::getline(buffer, line, delim); ) {
		if (!line.empty()) {
			result.push_back(line);
		}
	}
	return result;
}


std::string Data::Join(const std::vector<std::string> & input)
{
	std::stringstream buffer;
	for (const std::string &s : input) {
		buffer << s;
	}
	return buffer.str();
}


std::vector<Variable> Data::ParseVariables(const std::vector<std::string> & tokens, bool opposite)
{
	using std::vector;
	using std::string;
	using std::regex;

	vector<Variable> variables;

	for (const string & token : tokens) {

		bool parseFail = false;
		int sign, coefficient, index;

		std::smatch sm;
		bool result = std::regex_search(token, sm, regex("([\\+\\-]?)([0-9]*)C([0-9]+)"));
		if (result && sm.size() == 4) {
			// sign
			if (sm[1] == "+" || sm[1] == "") {
				sign = 1;
			} else if (sm[1] == "-") {
				sign = -1;
			} else {
				parseFail = true;
			}

			// check whether opposite
			if (opposite){
				sign *= -1;
			}

			// coefficient
			try {
				if (sm[2] == "") {
					coefficient = 1;
				} else {
					coefficient = std::stoi(sm[2]);
				}
			} catch (std::exception e) {
				parseFail = true;
			}

			// index
			try {
				index = std::stoi(sm[3]);
			} catch (std::exception e) {
				parseFail = true;
			}
		} else {
			parseFail = true;
		}

		if (parseFail) {
			throw new ParseException("'" + token + "' is not a valid variable");
		}

		variables.push_back(Variable(sign * coefficient, index));
	}

	return variables;
}


Condition Data::ParseExpression(const std::vector<std::string> & tokens_)
{
	using std::vector;
	using std::string;

	string tokensString = Data::Join(tokens_);
	tokensString = std::regex_replace(tokensString, std::regex("([\\+\\-])"), " $1");
	tokensString = std::regex_replace(tokensString, std::regex("(\\<\\=|\\>\\=|\\=)"), " $1 ");
	vector<string> tokens = Data::Split(tokensString, ' ');

	if (tokens.size() < 3) {
		throw new ParseException("'" + Data::Join(tokens) + "' is not a valid expression");
	}

	Condition::Type conditionType;
	vector<Variable> variables;
	int constant;

	const string & operatorString = tokens[tokens.size() - 2];
	const string & constantString = tokens[tokens.size() - 1];

	// variables
	variables = Data::ParseVariables(vector<string>(tokens.begin(), tokens.begin() + tokens.size() - 2));

	// =, <=, >=
	if (operatorString == "=") {
		conditionType = Condition::Type::eq;
	} else if (operatorString == "<=") {
		conditionType = Condition::Type::leq;
	} else if (operatorString == ">=") {
		conditionType = Condition::Type::geq;
	} else {
		throw new ParseException("'" + operatorString + "' is not a valid operator");
	}

	// constant
	try {
		constant = std::stoi(constantString);
	} catch (std::exception e) {
		throw new ParseException("'" + constantString + "' is not a valid integer");
	}

	std::sort(variables.begin(), variables.end());
	return Condition(conditionType, variables, constant);
}

size_t Data::ParseVariable(std::string variable)
{
	using std::string;
	using std::regex;

	std::smatch sm;
	bool result = std::regex_search(variable, sm, regex("C([0-9]+)"));
	bool parseFail = false;

	int index;

	if (result && sm.size() == 2) {
		try {
			index = std::stoi(sm[1]);
		}
		catch (std::exception e) {
			parseFail = true;
		}
	}
	else {
		parseFail = true;
	}

	if (parseFail) {
		throw new ParseException("'" + variable + "' is not a valid variable");
	}

	return index;
}

void Data::Parse(const std::string & input_)
{
	using std::vector;
	using std::string;
	using std::regex;
	using std::stringstream;

	string input = input_;
	//std::cout<<input<<std::endl;
	input = std::regex_replace(input, regex("/\\*(.|\n)*?\\*/"), ""); // remove comment
	input = std::regex_replace(input, regex("^\\s*$"), ""); // remove blank
	input = std::regex_replace(input, regex("[\n\r]"), " "); // remove line
	//std::cout<<input<<std::endl;
	stringstream buffer(input);
	for (string line; std::getline(buffer, line, ';'); ) {
		vector<string> tokens = Data::Split(line, ' ');
		if (!tokens.empty()) {
			if (tokens[0].size() >= 3 && tokens[0].substr(0, 3) == "int") {
				tokens.erase(tokens.begin());
				vector<string> vars = Data::Split(Data::Join(tokens), ',');
				for (const string & var : vars) {
					int index = ParseVariable(var);
					this->indices.push_back(index);
				}
			} else if (tokens[0].size() >= 4 && tokens[0].substr(0, 4) == "max:") {
				function = Data::ParseVariables(vector<string>(tokens.begin() + 1, tokens.end()));
			} else if (tokens[0].size() >= 4 && tokens[0].substr(0, 4) == "min:") {
				function = Data::ParseVariables(vector<string>(tokens.begin() + 1, tokens.end()), true);
			} else {
				if (tokens[0].back() == ':') {
					tokens.erase(tokens.begin());
				}
				Condition condition = ParseExpression(tokens);
				if (condition.variables.size() == 1) {
					Variable variable = condition.variables.front();
					Bounds bounds;

					/** coefficient * variable [= // <= // >=] constant
					 * if operator is =
					 * 		upper = lower = constant / coefficient
					 * if operator is >=
					 * 		if sign is +
					 * 			upper = +infinity
					 * 			lower = ceil(constant / coefficient)
					 * 		if sign is -
					 * 			upper = floor(constant / coefficient)
					 * 			lower = -infinity
					 * if operator is <=
					 * 		if sign is +
					 * 			upper = floor(constant / coefficient)
					 * 			lower = -infinity
					 * 		if sign is -
					 * 			lower = ceil(constant / coefficient)
					 * 			upper = +infinity
					 */
					int ceil_ = ceil(condition.constant*1.0 / variable.coefficient);
					int floor_ = floor(condition.constant*1.0 / variable.coefficient);

					switch (condition.type){
						case Condition::Type::eq:
							if (ceil_ == floor_) {
								bounds.upper = bounds.lower = ceil_;
							} else {
								// if ceil_ is not equal to floor_
								// the variable should be unsolvable.
								// simply let upper < lower
								bounds.upper = floor_;
								bounds.lower = ceil_;
							}
							break;

						case Condition::Type::leq:
							if (variable.coefficient < 0) {
								bounds.lower = ceil_;
							} else {
								bounds.upper = floor_;
							}
							break;

						case Condition::Type::geq:
							if (variable.coefficient < 0) {
								bounds.upper = floor_;								
							} else {
								bounds.lower = ceil_;
							}
							break;

						default:
							break;
					}

					this->bounds.push_back(std::pair<size_t, Bounds>(variable.index, bounds));
				} else {
					this->conditions.push_back(condition);
				}
			}
		}
	}

	std::sort(this->indices.begin(), this->indices.end());
	std::sort(this->bounds.begin(), this->bounds.end());
}


std::string Data::Print()
{
	using std::stringstream;
	using std::vector;

	stringstream output;

	vector<vector<int> > eq;
	vector<vector<int> > leq;

	// give a new index
	const size_t indexSize = this->indices.size();
	std::map<size_t, size_t> mapIndex;
	{
		int count = 0;
		for (size_t index : this->indices) {
			mapIndex[index] = count++;
		}
	}

	for (const Condition & condition : this->conditions) {
		vector<int> vec;
		vec.resize(indexSize, 0);

		for (const Variable & variable : condition.variables) {
			size_t index = mapIndex[variable.index];
			int coe = variable.coefficient;
			vec[index] = coe;
		}
		vec.push_back(condition.constant);

		switch (condition.type) {
			case Condition::Type::eq:
				eq.push_back(vec);
				break;

			case Condition::Type::leq:
				leq.push_back(vec);
				break;

			case Condition::Type::geq:
				for (int &e : vec) {
					e *= -1;
				}
				leq.push_back(vec);
				break;

			default:
				break;
		}
	}

	output << indexSize << std::endl;

	for (const auto & var : function){
		output << var.coefficient << " " << var.index << " ";
	}
	output << std::endl;
	
	output << this->bounds.size() << std::endl;
	for (const auto p : this->bounds) {
		size_t index = p.first;
		const Bounds & bounds = p.second;
		output << index << " " << bounds.lower << " " << bounds.upper << std::endl;
	}

	output << eq.size() << std::endl;
	for (const auto & vec : eq) {
		for (const int e : vec) {
			output << e << " ";
		}
		output << std::endl;
	}

	output << leq.size() << std::endl;
	for (const auto & vec : leq) {
		for (const int e : vec) {
			output << e << " ";
		}
		output << std::endl;
	}

	return output.str();
}
