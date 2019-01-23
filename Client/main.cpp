#define _WINSOCK_DEPRECATED_NO_WARNINGS // deprecated ó���� �� �Լ��� �״�� ����ϰڴٰ� ���
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[�����߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket; // ������ ����� �ڱ��ڽ��� ������ �ǹ�-> clientSocket
	SOCKADDR_IN serverAddress; // ������ �ּҸ� ������� SOCKADDR ����ü�� �̿��� �ϳ��� ������ �������
	
	int serverPort = 9876; // SERVER�� ������������ ��Ʈ��ȣ�� ��� Because, Ư���� ������ �������α׷����� ��û�� �� �� ����
	char received[256]; // 256����Ʈ�� ����ִ� received��� ���ڿ� ���۸� ����
	string sent; // sent�� ����ڰ� �Է��� ���ڿ��� ������ �����ֱ� ���ؼ� �ϳ��� string������ �������

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // *Winsock�� �ʱ�ȭ ��*
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // *TCP������ ����* // SERVER�� ������ �ڵ�, But ���⼭�� clientSocket�� Ŭ���̾�Ʈ�� ����ϴ� ���Ͽ� ���� ������

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("soket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // �����ּҸ� 0���� �ʱ�ȭ�� ���Ŀ�, ������ ���� ������ �־���
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ������ �ּҰ��� �ʱ�ȭ�� �� inet_addr(��� ���������� �Ű������� ���ڿ��� ���޹���)�� IP�ּҷε� ���ڿ��� ���޹޾� ���. '127.0.0.1'�� �ڱ��ڽ���ǻ�͸� �ǹ��ϴ� ��ӵ� IP�ּ�// *���ڿ�ip�� ��ũ��ũ ����Ʈ ��������* // 127.0.0.1(�����ּ�)�� serverAddress�� �ּҰ����� �����ϱ� ���ؼ� inet_addr��� �Լ��� �ҷ���(�� �Լ��� ����ϱ� ���ؼ� ���� 1���� ���������)
	serverAddress.sin_port = htons(serverPort); // *2����Ʈ ���� ��ũ��ũ ����Ʈ ��������*
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // connect��ɾ �����Ͽ� �������� �����û�� ����(-> ����������  �� connect�� accept�Լ��� ����)
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	while (1) { // *�ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���� ����*
		cout << "[�޼��� ����]: ";
		getline(cin, sent);
		if (sent == " ") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0); // ����ڰ� �Է��� ���ڿ��� clientSocket�� ���� ������ ���� �� ���� // sent�� ���ڿ����� string���� ������⶧����, c����� ���ڿ��������·� �ٲپ� �ֱ� ���ؼ� c_str()�Լ��� �ҷ��;���  
		int length = recv(clientSocket, received, sizeof(received), 0); // ������ ���� ����ó���� ���ڿ��� �״�� ����������ν�, received�� �̿��� ������ ���� ���޹��� �� ���ڿ��� received���ۿ� ���� �� ����
		received[length] = '\0'; // 8���ڿ� ���������� NULL���� �־ ���ڿ��� ȿ�������� ó���� �� �ֵ��� ��
		if (strcmp(received, "[exit]") == 0) { // ������ ���� ���޹��� ���� exit��ɾ���, 
			cout << "[���� ����]\n";
			break; // ���α׷��� ������
		}
		cout << "[���� �޼���]: " << received << '\n'; // exit�� �ƴ϶�� ���޹��� �޼����� �״�� ��������ν�, ����Ŭ���̾�Ʈ�� ������ ����
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}