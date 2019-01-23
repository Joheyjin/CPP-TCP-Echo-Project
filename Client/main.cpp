#define _WINSOCK_DEPRECATED_NO_WARNINGS // deprecated 처리가 된 함수를 그대로 사용하겠다고 명시
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket; // 서버와 연결된 자기자신의 소켓을 의미-> clientSocket
	SOCKADDR_IN serverAddress; // 서버의 주소를 담기위해 SOCKADDR 구조체를 이용해 하나의 변수를 만들어줌
	
	int serverPort = 9876; // SERVER와 완전히동일한 포트번호를 사용 Because, 특정한 서버의 서버프로그램에게 요청을 할 수 있음
	char received[256]; // 256바이트를 담고있는 received라는 문자열 버퍼를 만듦
	string sent; // sent는 사용자가 입력한 문자열을 서버로 보내주기 위해서 하나의 string변수를 만들어줌

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // *Winsock을 초기화 함*
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // *TCP소켓을 생성* // SERVER와 동일한 코딩, But 여기서의 clientSocket은 클라이언트가 사용하는 소켓에 대한 정보임

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("soket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // 서버주소를 0으로 초기화한 이후에, 서버에 대한 정보를 넣어줌
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 서버의 주소값을 초기화할 때 inet_addr(얘는 내부적으로 매개변수를 문자열로 전달받음)로 IP주소로된 문자열을 전달받아 사용. '127.0.0.1'은 자기자신컴퓨터를 의미하는 약속된 IP주소// *문자열ip를 네크워크 바이트 형식으로* // 127.0.0.1(서버주소)을 serverAddress의 주소값으로 전달하기 위해서 inet_addr라는 함수를 불러옴(이 함수를 사용하기 위해선 위의 1행을 적어줘야함)
	serverAddress.sin_port = htons(serverPort); // *2바이트 정수 네크워크 바이트 형식으로*
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // connect명령어를 수행하여 서버에게 연결요청을 보냄(-> 서버에서는  이 connect를 accept함수로 받음)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	while (1) { // *반복적으로 서버로 메세지를 전송하고 에코 메세지를 전달 받음*
		cout << "[메세지 전송]: ";
		getline(cin, sent);
		if (sent == " ") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0); // 사용자가 입력한 문자열을 clientSocket을 통해 서버로 보낼 수 있음 // sent를 문자열변수 string으로 만들었기때문에, c언어의 문자열버퍼형태로 바꾸어 주기 위해선 c_str()함수를 불러와야함  
		int length = recv(clientSocket, received, sizeof(received), 0); // 서버로 부터 에코처리된 문자열이 그대로 날라들어옴으로써, received를 이용해 서버로 부터 전달받은 그 문자열을 received버퍼에 담을 수 있음
		received[length] = '\0'; // 8문자열 마지막에는 NULL값을 넣어서 문자열을 효과적으로 처리할 수 있도록 함
		if (strcmp(received, "[exit]") == 0) { // 서버로 부터 전달받은 값이 exit명령어라면, 
			cout << "[서버 종료]\n";
			break; // 프로그램을 종료함
		}
		cout << "[서버 메세지]: " << received << '\n'; // exit가 아니라면 전달받은 메세지를 그대로 출력함으로써, 에코클라이언트를 개발한 것임
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}