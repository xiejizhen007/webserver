#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    int fd = open("../resource/file", O_RDWR);
    char buf0[1024];
    char buf1[1024];

    iovec iov[2];

    iov[0].iov_base = buf0;
    // iov[0].iov_len = sizeof(buf0);
    iov[0].iov_len = 1024;
    iov[1].iov_base = buf1;
    iov[1].iov_len = sizeof(buf1);

    int len = readv(fd, iov, 2);
    cout << "len: " << len << endl;
    cout << buf0 << endl;
    cout << "next" << endl;
    cout << buf1 << endl;
}