
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

char c_response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";

struct Client
{
    int client_fd;
    int request_number;
};

void* request_handle(void* arg)
{
    Client* client = (Client*)arg;

    string response = c_response;
    response += "Request number: " + std::to_string(client->request_number) + '\n';

    char buffer[128];
    FILE* pipe = popen("php version.php", "r");
    while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        response += buffer;
    }

    response += '\n';

    pclose(pipe);

    write(client->client_fd, response.c_str(), response.size() * sizeof(char));
    
    shutdown(client->client_fd, SHUT_WR);

    delete client;

    pthread_exit(NULL);
}

int main()
{
    int request_number = 1;
    int one = 1, client_fd;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof(cli_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        err(1, "can't open socket");
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    int port = 8080;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1)
    {
        close(sock);
        err(1, "Can't bind");
    }

    listen(sock, 5);
    Client* client;

    while (1)
    {
        client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
        cout << "got connection" << request_number << '\n';

        if (client_fd == -1)
        {
            cerr << "Can't accept";
            continue;
        }

        pthread_t thread;
        int err;
        client = new Client;
        client->request_number = request_number;
        client->client_fd = client_fd;
        // Создаём поток
        err = pthread_create(&thread, NULL, request_handle, (void*)client);
        // Если при создании потока произошла ошибка, выводим
        // сообщение об ошибке и прекращаем работу программы
        if(err != 0)
        {
            cout << "Cannot create a thread: " << strerror(err) << endl;
            exit(-1);
        }

        ++request_number;

        pthread_detach(thread);
    }
}