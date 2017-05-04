//
// Created by root on 17-5-2.
//

#include "Logging.hpp"

Kanna::Logging::Logging() {

}

Kanna::Logging::Logging(std::string logfile) {
	LogFile = fopen(logfile.c_str(), "a");

	if (!LogFile)
		throw Reimu::Exception(errno);

}

void Kanna::Logging::Log(int type, const char *format, ...) {
	char *tmpbuf = NULL;
	size_t tmpbuf_size = 0;

	FILE *tmpbufp = open_memstream(&tmpbuf, &tmpbuf_size);

	time_t now = time(NULL);
	char timestr[20];
	memset(timestr, 0, 20);
	strftime(timestr, 20, "%F %T", localtime(&now));


	va_list ap;

	va_start(ap, format);
	vfprintf(tmpbufp, format, ap);
	va_end(ap);

	fprintf(tmpbufp, "\n");
	fclose(tmpbufp);

	const char *color = "1";

	if (type <= 20)
		color = "1";
	else if (type <= 40)
		color = "3";
	else if (type <= 50)
		color = "2";
	else if (type <= 60)
		color = "5";

	fprintf(stderr, "\e[01;36m[%s]\e[0m ", timestr);
	fprintf(stderr, "\e[01;3%sm%s\e[0m", color, tmpbuf);

	pthread_mutex_lock(&Lock);
	fprintf(LogFile, "[%s] ", timestr);
	fwrite(tmpbuf, 1, tmpbuf_size, LogFile);
	fflush(LogFile);
	pthread_mutex_unlock(&Lock);

	free(tmpbuf);
}

std::stringstream &Kanna::Logging::operator<<(int type) {
//	SSBuf << _arg.operator std::string();

	std::cout << "xx " << SSBuf.str() << "\n";


	return SSBuf;
}
