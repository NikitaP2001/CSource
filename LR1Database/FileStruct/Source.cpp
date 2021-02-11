#include "FileReader.h"
struct File {
public:
	File(const char* _inf, const char* _libr) {
		inf = new FileReader(_inf);
		libr = new FileReader(_libr);
		Items_Count = inf->ReadInt(0);
	}
	void process_command(const char* choices, size_t choices_num) {

	}
	FileReader* inf;
	FileReader* libr;
protected:
	char* getword(const char* target, int pos) {
		char res[24];
		memset(res, 0, sizeof(res));
		int wordc = 0;
		int tarp = 0;
		int resp = 0;
		while ((target[tarp] != 0) && (wordc <= pos)) 
		{
			if (target[tarp] != ' ') {
				res[resp] = target[tarp];
				resp++;
			}
			else if (wordc != pos) {
				memset(res, 0, sizeof(res));
				resp = 0;
				wordc++;
			}
			else wordc++;
			tarp++;
		}
		return res;
	}

	char* readword(int pos, int size) {
		char res[24];
		memset(res, 0, sizeof(res));
		int resp = 0;
		for (pos; resp < size; pos++ , resp++) {
			res[resp] = libr->Read(pos);
		}
		return res;
	}

	void clearsub(int offset, int size) {
		libr->clear(offset, size);
	}

	int Items_Count = 0;
};
struct Slave : public File
{
	Slave(const char* _inf, const char* _libr) : File(_inf, _libr) {
	}
public:
	void process_command(const char* choice) {

		if (strcmp("insert-s", getword(choice, 0)) == 0) {
			char res[24];
			int i = 4;
			int place = 4;
			boolean found = 0;
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);
			int PId = 0;
			intPid >> PId;
			int size = inf->size();
			int flag1 = 0;
			while (i + 4 < size) {		//find out free place in inf file
				if ((inf->ReadInt(i) == 0) && !flag1) {
					place = i;
					flag1 = 1;
				}
				if (inf->ReadInt(i) == PId) {
					found = 1;
				}
				i = i + 4;
			}
			if (found) {
				std::cout << "Sub-record already exist!" << std::endl;
			}
			else {
				inf->WriteInt(0, inf->ReadInt(0) + 1);
				i = place;
				strcpy(res, getword(choice, 1));
				std::stringstream intPid(res);			//write pid to inf file
				int number = 0;
				intPid >> number;
				inf->WriteInt(i, number);
				i = ((i - 4) / 4) * 32;
				strcpy(res, getword(choice, 2));
				std::stringstream intBook(res);			//get BookId
				number = 0;
				intBook >> number;
				libr->WriteInt(i, number);
				i = i + 4;
				strcpy(res, getword(choice, 3));			//get Office
				libr->Write(res, i);
				i = i + 24;
				strcpy(res, getword(choice, 4));
				std::stringstream intYear(res);			//get Year
				number = 0;
				intYear >> number;
				libr->WriteInt(i, number);
			}
		}

		if (strcmp("get-s", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Bookid to int
			int PId = 0;
			intPid >> PId;
			Gets(PId);
		}

		if (strcmp("ut-s", getword(choice, 0)) == 0) {
			char res[24];
			int i = 4;
			int temp;
			int found = 0;
			int RecordCount = inf->ReadInt(0);
			while ((RecordCount != found) && (i + 4 < libr->size())) {		//find PId
				if ((temp = inf->ReadInt(i)) != 0) {
					found++;
					Gets(temp);
				}
				i = i + 4;
			}
		}

		if (strcmp("del-s", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Pkid to int
			int PId = 0;
			intPid >> PId;
			Dels(PId);
		}

		if (strcmp("del-m", getword(choice, 0)) == 0) {
			char res[24];
			int i = 4;
			int temp;
			int found = 0;
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//write pid to inf file
			int number = 0;
			intPid >> number;
			int RecordCount = inf->ReadInt(0);
			while ((RecordCount != found) && (i + 4 < inf->size())) {		//find PId
				if ((temp = inf->ReadInt(i)) != 0) {
					found++;
					if (GetBid(temp) == number) {
						inf->WriteInt(0, inf->ReadInt(0) - 1);
						inf->WriteInt(i, 0);
					}
				}
				i = i + 4;
			}
		}

		if (strcmp("calc-s", getword(choice, 0)) == 0) {
			std::cout << inf->ReadInt(0) << " subrecords" << std::endl;
		}

		if (strcmp("update-s", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Bookid to int
			int PId = 0;
			intPid >> PId;
			int i = 4;
			while ((inf->ReadInt(i) != PId) && (i + 4 < inf->size())) {		//find BookId
				i = i + 4;
			}
			if (i + 4 >= inf->size()) {
				std::cout << "Cant find record" << std::endl;
			}
			else {
				i = ((i - 4) / 4) * 32;
				if (strcmp("BookID", getword(choice, 2)) == 0) {
					strcpy(res, getword(choice, 3));
					std::stringstream intID(res);			//get BookID
					int number = 0;
					intID >> number;
					libr->WriteInt(i, number);
					std::cout << "BookID changed to: " << number << std::endl;
				}
				i = i + 4;
				if (strcmp("OfficeName", getword(choice, 2)) == 0) {
					clearsub(i, 24);
					strcpy(res, getword(choice, 3));			//get Office
					libr->Write(res, i);
					std::cout << "Office changed to: " << res << std::endl;
				}
				i = i + 24;
				if (strcmp("YearPublished", getword(choice, 2)) == 0) {
					strcpy(res, getword(choice, 3));
					std::stringstream intYear(res);			//get year
					int number = 0;
					intYear >> number;
					libr->WriteInt(i, number);
					std::cout << "Year changed to: " << res << std::endl;
				}
			}
		}
	}
private:
	void Gets(int PId) {
		char res[24];
		int i = 4;
		while ((inf->ReadInt(i) != PId) && (i + 4 < inf->size())) {		//find PId
			i = i + 4;
		}
		if (i + 4 >= inf->size()) {
			std::cout << "Cant find sub-record" << std::endl;
		}
		else {
			std::cout << "PId: " << PId << " ";
			i = ((i - 4) / 4) * 32;					//point on the lib file
			int num = libr->ReadInt(i);			//get BookID
			std::cout << "BookID: " << num << " ";
			i = i + 4;
			strcpy(res, readword(i, 24));			//get PublishingOffice
			std::cout << "OfficeName: " << res << " ";
			i = i + 24;
			num = libr->ReadInt(i);			//get Year
			std::cout << "YearPublished: " << num << " " << std::endl;
		}
	}

	int GetBid(int PId) {
		char res[24];
		int i = 4;
		while ((inf->ReadInt(i) != PId) && (i + 4 < inf->size())) {		//find PId
			i = i + 4;
		}
		if (i + 4 >= inf->size()) {
		}
		else {
			i = ((i - 4) / 4) * 32;					//point on the lib file
			int num = libr->ReadInt(i);			//get BookID
			return num;
		}
	}

	void Dels(int PId) {
		char res[24];
		int i = 4;
		while ((inf->ReadInt(i) != PId) && (i + 4 < inf->size())) {		//find PId
			i = i + 4;
		}
		if (i + 4 >= inf->size()) {
			std::cout << "Cant find record" << std::endl;
		}
		else {
			inf->WriteInt(i, 0);
			inf->WriteInt(0, inf->ReadInt(0) - 1);
			std::cout << PId << " Deleted" << std::endl;
		}
	}
};
struct Master : public File
{
	Master(const char* _inf, const char* _libr) : File(_inf, _libr) {
	}
public:
	void process_command(const char* choice) {

		if (strcmp("insert-m", getword(choice, 0)) == 0) {
			char res[24];
			int i = 4;
			int place = 4;
			boolean found = 0;
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);
			int BookId = 0;
			intPid >> BookId;
			int size = inf->size();
			int flag1 = 0;
			while (i + 4 < size) {		//find out free place in inf file
				if ((inf->ReadInt(i) == 0) && !flag1) {
					place = i;
					flag1 = 1;
				}
				if (inf->ReadInt(i) == BookId) {
					found = 1;
				}
				i = i + 4;
			}
			if (found) {
				std::cout << "Record already exist!" << std::endl;
			}
			else {
				inf->WriteInt(0, inf->ReadInt(0) + 1);
				i = place;
				strcpy(res, getword(choice, 1));
				std::stringstream intPid(res);			//write pid to inf file
				int number = 0;
				intPid >> number;
				inf->WriteInt(i, number);
				i = ((i - 4) / 4) * 56;
				clearsub(i, 56);
				strcpy(res, getword(choice, 2));			//get Name
				libr->Write(res, i);
				i = i + 24;
				strcpy(res, getword(choice, 3));
				std::stringstream intYear(res);			//get Year
				number = 0;
				intYear >> number;
				libr->WriteInt(i, number);
				i = i + 4;
				strcpy(res, getword(choice, 4));			//get Genre
				libr->Write(res, i);
				i = i + 24;
				strcpy(res, getword(choice, 5));
				std::stringstream intISBN(res);			//get ISBN
				number = 0;
				intISBN >> number;
				libr->WriteInt(i, number);
			}
		}

		if (strcmp("get-m", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Bookid to int
			int BookId = 0;
			intPid >> BookId;
			Getm(BookId);
		}

		if (strcmp("ut-m", getword(choice, 0)) == 0){
			char res[24];
			int i = 4;
			int temp;
			int found = 0;
			int RecordCount = inf->ReadInt(0);
			while ((RecordCount != found) && (i + 4 < libr->size())) {		//find BookId
				if ((temp = inf->ReadInt(i)) != 0) {
					found++;
					Getm(temp);
				}
				i = i + 4;
			}
		}

		if (strcmp("del-m", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Bookid to int
			int BookId = 0;
			intPid >> BookId;
			Delm(BookId);
		}

		if (strcmp("calc-m", getword(choice, 0)) == 0) {
			std::cout << inf->ReadInt(0) << " records" << std::endl;
		}

		if (strcmp("update-m", getword(choice, 0)) == 0) {
			char res[24];
			strcpy(res, getword(choice, 1));
			std::stringstream intPid(res);			//Bookid to int
			int BookId = 0;
			intPid >> BookId;
			int i = 4;
			while ((inf->ReadInt(i) != BookId) && (i + 4 < inf->size())) {		//find BookId
				i = i + 4;
			}
			if (i + 4 >= inf->size()) {
				std::cout << "Cant find record" << std::endl;
			}
			else {
				i = ((i - 4) / 4) * 56;
				if (strcmp("Name", getword(choice, 2)) == 0) {
					clearsub(i, 24);
					strcpy(res, getword(choice, 3));			//get Name
					libr->Write(res, i);
					std::cout << "Name changed to: " << res << std::endl;
				}
				i = i + 24;
				if (strcmp("Year", getword(choice, 2)) == 0) {
					strcpy(res, getword(choice, 3));
					std::stringstream intYear(res);			//get Year
					int number = 0;
					intYear >> number;
					libr->WriteInt(i, number);
					std::cout << "Year changed to: " << number << std::endl;
				}
				i = i + 4;
				if (strcmp("Genre", getword(choice, 2)) == 0) {
					clearsub(i, 24);
					strcpy(res, getword(choice, 3));			//get Genre
					libr->Write(res, i);
					std::cout << "Genre changed to: " << res << std::endl;
				}
				i = i + 24;
				if (strcmp("ISBN", getword(choice, 2)) == 0) {
					strcpy(res, getword(choice, 3));
					std::stringstream intYear(res);			//get ISBN
					int number = 0;
					intYear >> number;
					libr->WriteInt(i, number);
					std::cout << "ISBN changed to: " << res << std::endl;
				}
			}
		}
	}
private:
	void Getm(int BookId) {
		char res[24];
		int i = 4;
		while ((inf->ReadInt(i) != BookId) && (i + 4 < inf->size())) {		//find BookId
			i = i + 4;
		}
		if (i + 4 >= inf->size()) {
			std::cout << "Cant find record" << std::endl;
		}
		else {
			std::cout << "BookId: " << BookId << " ";
			i = ((i - 4) / 4) * 56;					//point on the lib file
			strcpy(res, readword(i, 24));			//get Name
			std::cout << "Name: " << res << " ";
			i = i + 24;
			int num = libr->ReadInt(i);			//get Year
			std::cout << "Year: " << num << " ";
			i = i + 4;
			strcpy(res, readword(i, 24));			//get Genre
			std::cout << "Genre: " << res << " ";
			i = i + 24;
			num = libr->ReadInt(i);			//get ISBN
			std::cout << "ISBN: " << num << std::endl;
		}
	}

	void Delm(int BookId) {
		char res[24];
		int i = 4;
		while ((inf->ReadInt(i) != BookId) && (i + 4 < inf->size())) {		//find BookId
			i = i + 4;
		}
		if (i + 4 >= inf->size()) {
			std::cout << "Cant find record" << std::endl;
		}
		else {
			inf->WriteInt(i, 0);
			inf->WriteInt(0, inf->ReadInt(0) - 1);
			std::cout << BookId << " Deleted" << std::endl;
		}
	}
};
int main() {
	Master m("C:\\Users\\Nikita\\Documents\\CProj\\FileStruct\\Debug\\b.inf",
		"C:\\Users\\Nikita\\Documents\\CProj\\FileStruct\\Debug\\b.libr");
	Slave s("C:\\Users\\Nikita\\Documents\\CProj\\FileStruct\\Debug\\p.inf",
		"C:\\Users\\Nikita\\Documents\\CProj\\FileStruct\\Debug\\p.libr");
	char input[124];
	while (std::cin.getline(input, 124)) {
		m.process_command(input);
		s.process_command(input);
	}
	system("pause");
	return 0;
}