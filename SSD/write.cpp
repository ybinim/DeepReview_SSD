#include <string>
#include <map>

class Write {
public:
	bool execute(const std::map<std::string, std::string> &nand, const std::string &lba, const std::string &data) {
		std::map<std::string, std::string> mapData = nand;
		auto ret = mapData.emplace(lba, data);

		if (ret.second) {
			return true;
		}
		return 0;
	}
};