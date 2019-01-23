#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) // 오류상황이 발생하면, 메시지를 출력하고 프로그램을 종료
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaDate; // winsock API의  데이터구조체를 하나의 변수로 만들어줌
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	// 이러한 구조체들은 보통 winsock라이브러리에 포함되어있다는 특징이있음

	int serverPort = 9876;
	char received[256]; // 클라이언트와 데이터를 주고받을 때, 문자열 버퍼의 크기를 256으로 설정

	if (WSAStartup(MAKEWORD(2, 2), &wsaDate) != 0) // *Winsock을 초기화 합니다 // 2.2버전을 사용하겠다고 MAKEWORD라는 구조체를 활용하고, Startup된 결과를 윈속API의 wsaData에 저장하도록 함
		ShowErrorMessage("WSAStartup()"); // 0이아니라면 오류메시지를 반환

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // *TCP소켓을 생성합니다->SOCK_STREAM, TCP형태로 서버를 운영하겠다고 명시 // 서버 소켓을 초기화(PF_INET은 밑의 AF_INET과 동일하며, IPv4를 사용하겠다고 명시한 것임) 
	
	if (serverSocket == INVALID_SOCKET) // 예외처리
		ShowErrorMessage("soket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // serverAddress를 0이라는 값으로 초기화하고, 밑에서는 값을 채워줄 것임
	serverAddress.sin_family = AF_INET; // sin_family -> (어떠한 체계를 사용할 것인지)IPv4를 사용하겠다고 명시
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // serverAddress는 네트워크 바이트형식의 데이터포함하며, 이를 위해 htonl함수 사용(l은 long)하여, *4바이트 정수를 네트워크 바이트 형식으로 담아줌*// 서버 본연의 주소로써 자신의 주소를 활용해 서버를 열겠다고 주소값을 적음
	serverAddress.sin_port = htons(serverPort); // htons(s는 shot)을 이용해 *2바이트 정수 네트워크 바이트 형식으로* 포트번호를 넣어줌
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // 해당서버소켓으로 bind처리를 해서, 서버프로그램을 실행하고 있는 컴퓨터에 네트워크인터페이스가 바인딩이 될수 있도록 처리함. 바인딩 과정을 위해 serverAddress가 들어가있음
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // 동시에 연결을 요청하는 client의 숫자가 총5개까지 들어갈 수 있게 함
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress); // clientAdress를 이용하여
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // accept함수를 수행해, 클라이언트의 요청을 기다림 -> 클라이언트에 대한 정보가 clientAdress에 담기게 됨 => 즉, 해당클라이언트와 통신할 수 있는 소켓은 clientSocket에 들어가게 되고, 이 clientSocket을 이용해 연결을 수행한 클라이언트와 데이터를 주고받을 수 있음 

	if (clientSocket == INVALID_SOCKET) 
		ShowErrorMessage("accept()");
	
	while(1) { // *클라이언트의 메세지를 받아 그대로 다시 전달함* // 서버 프로그램은 무한반복을 수행 => 클라이언트가 서버에 접속한 경우, 계속 무한반복을 하며 클라이언트로 부터 데이터를 계속 받으면서 전달받은 메세지를 다시 클라이언트에게 보내줌
		int length = recv(clientSocket, received, sizeof(received), 0); // 문자열버퍼에 클라이언트가 보낸 데이터가 담기게 되는데,  
		received[length] = NULL; // 마지막 문자를 NULL로 바꾸어서, 예상치 못한 문자가 들어왔을때 그 문자가 잘라내지도록 만듦
		cout << "[클라이어트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n'; // 1.클라이언트가 메세지를 전송할 때에는 엔터를 치고 전송함으로
		if (strcmp(received, "[exit]") == 0) { // 11.사용자가 exit이란 명령어를 보낸다면
			send(clientSocket, received, sizeof(received) - 1, 0); // 1.마지막 공백문자는 제거할 수 있도록 함
			cout << "[서버 종료]\n"; // 11.서버프로그램을 종료하도록함
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // exit가 아니라면 바로 client한테 client가 보낸 메세지를 그대로 전송 => 에코프로그램
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
} // 기본적인 형식을 그대로 따른 TCP 에코 서버프로그램이다.
