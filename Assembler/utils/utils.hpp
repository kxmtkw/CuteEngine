
#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <string>

namespace ctUtils {

	class ErrorCollector {
		std::string errors;
		bool has_errors = false;

		public:

		ErrorCollector() = default;
		void reset() {has_errors = false;}
		void append(std::string& err) {errors.append(err); has_errors = true;}
		bool hasErrors() {return has_errors;}
		std::string getErrors() {return errors;}
	};

}

#endif // UTILS_UTILS_HPP