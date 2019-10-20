#include "ASerial.h"
#include "Enumerations.h"
#include <stdexcept>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstring>

bool ASerial::canRead()
{
	if(_fileDescriptor < 0)
		return false;

	int bytes;

	ioctl(_fileDescriptor, FIONREAD, &bytes);

	return bytes >= 1;
}

CoinType ASerial::getCommand()
{
	if(_fileDescriptor < 0)
		return CoinType::Invalid;

	char buffer[1];
	int response = 0;

	int count = read(_fileDescriptor, buffer, 1);

	printf("%d\t%d\n", count, buffer[0]);

	if(count < 0)
		printf("error=%s\n", strerror(errno));

	response = buffer[0] - '0';

	return static_cast<CoinType>(response);
}

bool ASerial::openPort(const std::string& deviceName)
{
	if(_fileDescriptor >= 0)
	{
		fprintf(stderr, "Already open\n");
		return false;
	}

	int fd = open(_deviceName.c_str(), O_RDONLY | O_NOCTTY);

	if (fd < 0)
	{
		fprintf(stderr, "Unable to open serial port!\n");
		return false;
	}

	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |= CS8;      /* Set the data bits = 8                                 */

	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */


	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;	/*No Output Processing*/

	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 1;	
	SerialPortSettings.c_cc[VTIME] = 1; // should wait 0.1 seconds to read

	if (tcsetattr(fd, TCSANOW, &SerialPortSettings) < 0)
	{
		throw fprintf(stderr, "Cannot set attributes on serial port. %s\n", strerror(errno));
		return false;
	}

	tcflush(fd, TCIFLUSH);

	_fileDescriptor = fd;
	return true;
}

void ASerial::closePort()
{
	fprintf(stderr, "Not open\n");
	close(_fileDescriptor);
}
