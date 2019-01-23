#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) // ������Ȳ�� �߻��ϸ�, �޽����� ����ϰ� ���α׷��� ����
{
	cout << "[�����߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaDate; // winsock API��  �����ͱ���ü�� �ϳ��� ������ �������
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	// �̷��� ����ü���� ���� winsock���̺귯���� ���ԵǾ��ִٴ� Ư¡������

	int serverPort = 9876;
	char received[256]; // Ŭ���̾�Ʈ�� �����͸� �ְ���� ��, ���ڿ� ������ ũ�⸦ 256���� ����

	if (WSAStartup(MAKEWORD(2, 2), &wsaDate) != 0) // *Winsock�� �ʱ�ȭ �մϴ� // 2.2������ ����ϰڴٰ� MAKEWORD��� ����ü�� Ȱ���ϰ�, Startup�� ����� ����API�� wsaData�� �����ϵ��� ��
		ShowErrorMessage("WSAStartup()"); // 0�̾ƴ϶�� �����޽����� ��ȯ

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // *TCP������ �����մϴ�->SOCK_STREAM, TCP���·� ������ ��ϰڴٰ� ��� // ���� ������ �ʱ�ȭ(PF_INET�� ���� AF_INET�� �����ϸ�, IPv4�� ����ϰڴٰ� ����� ����) 
	
	if (serverSocket == INVALID_SOCKET) // ����ó��
		ShowErrorMessage("soket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // serverAddress�� 0�̶�� ������ �ʱ�ȭ�ϰ�, �ؿ����� ���� ä���� ����
	serverAddress.sin_family = AF_INET; // sin_family -> (��� ü�踦 ����� ������)IPv4�� ����ϰڴٰ� ���
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // serverAddress�� ��Ʈ��ũ ����Ʈ������ �����������ϸ�, �̸� ���� htonl�Լ� ���(l�� long)�Ͽ�, *4����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� �����*// ���� ������ �ּҷν� �ڽ��� �ּҸ� Ȱ���� ������ ���ڴٰ� �ּҰ��� ����
	serverAddress.sin_port = htons(serverPort); // htons(s�� shot)�� �̿��� *2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������* ��Ʈ��ȣ�� �־���
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // �ش缭���������� bindó���� �ؼ�, �������α׷��� �����ϰ� �ִ� ��ǻ�Ϳ� ��Ʈ��ũ�������̽��� ���ε��� �ɼ� �ֵ��� ó����. ���ε� ������ ���� serverAddress�� ������
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // ���ÿ� ������ ��û�ϴ� client�� ���ڰ� ��5������ �� �� �ְ� ��
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress); // clientAdress�� �̿��Ͽ�
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // accept�Լ��� ������, Ŭ���̾�Ʈ�� ��û�� ��ٸ� -> Ŭ���̾�Ʈ�� ���� ������ clientAdress�� ���� �� => ��, �ش�Ŭ���̾�Ʈ�� ����� �� �ִ� ������ clientSocket�� ���� �ǰ�, �� clientSocket�� �̿��� ������ ������ Ŭ���̾�Ʈ�� �����͸� �ְ���� �� ���� 

	if (clientSocket == INVALID_SOCKET) 
		ShowErrorMessage("accept()");
	
	while(1) { // *Ŭ���̾�Ʈ�� �޼����� �޾� �״�� �ٽ� ������* // ���� ���α׷��� ���ѹݺ��� ���� => Ŭ���̾�Ʈ�� ������ ������ ���, ��� ���ѹݺ��� �ϸ� Ŭ���̾�Ʈ�� ���� �����͸� ��� �����鼭 ���޹��� �޼����� �ٽ� Ŭ���̾�Ʈ���� ������
		int length = recv(clientSocket, received, sizeof(received), 0); // ���ڿ����ۿ� Ŭ���̾�Ʈ�� ���� �����Ͱ� ���� �Ǵµ�,  
		received[length] = NULL; // ������ ���ڸ� NULL�� �ٲپ, ����ġ ���� ���ڰ� �������� �� ���ڰ� �߶������� ����
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n'; // 1.Ŭ���̾�Ʈ�� �޼����� ������ ������ ���͸� ġ�� ����������
		if (strcmp(received, "[exit]") == 0) { // 11.����ڰ� exit�̶� ��ɾ �����ٸ�
			send(clientSocket, received, sizeof(received) - 1, 0); // 1.������ ���鹮�ڴ� ������ �� �ֵ��� ��
			cout << "[���� ����]\n"; // 11.�������α׷��� �����ϵ�����
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // exit�� �ƴ϶�� �ٷ� client���� client�� ���� �޼����� �״�� ���� => �������α׷�
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
} // �⺻���� ������ �״�� ���� TCP ���� �������α׷��̴�.
