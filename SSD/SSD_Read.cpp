#include <iostream>
#include <string>
#include <map>
#include <fstream>

#if 0
Read
• ssd R[LBA]
• ssd_output.txt 에 읽은 값이 적힌다. 기존 데이터는 사라진다.
• 기록이 한적이 없는 LBA 를 읽으면 0x00000000 으로 읽힌다
• 잘못된 LBA 범위가 입력되면 ssd_output.txt 에 ERROR 가 기록된다
#endif

class Read {
public:
	void execute(std::map<int, std::string>& nand, int addr) {
		// ofstream 객체 생성 (파일이 없다면 생성되고, 있으면 열립니다)
		std::ofstream outFile(filePath, std::ios::trunc);

		// 파일 열기 실패 체크
		if (!outFile) {
			std::cout << "파일을 열 수 없습니다!" << std::endl;
			return;
		}

		if (addr < 0 || addr > 100) {
			// 잘못 된 주소 일 때 ERROR 기록하기
			outFile << "ERROR";
			outFile.close();
			return;
		}

		auto it = nand.find(addr);

		// 키가 존재하지 않는다면, 0x00000000 쓰기
		if (it == nand.end()) {
			outFile << "0x00000000";
		}
		else {
			outFile << it->second;
		}

		// 파일 닫기
		outFile.close();
		return;
	}

private:
	std::string filePath = "ssd_output.txt";
};