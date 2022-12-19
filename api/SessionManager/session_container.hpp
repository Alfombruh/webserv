#ifndef SESSION_CONTAINER
#define SESSION_CONTAINER

#include <iostream>

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;

namespace session
{
	// PRIVATE
	namespace
	{

	}
	// PUBLIC
	typedef struct
	{
		string hash;
		string name;
		string about;
	} session;

	void createSession(const session &session)
	{
		ofstream sessionFile(("./api/SessionManager/sess_" + session.hash).c_str(), std::ofstream::out | std::ofstream::trunc);
		sessionFile << session.name << "\n"
					<< session.about << "\n";
		sessionFile.close();
	};

	void removeSession(const string &hash)
	{
		std::remove(("./api/SessionManager/sess_" + hash).c_str());
	};

	const session getSession(const string &hash)
	{
		string line;
		session session = {.hash = hash};
		ifstream sessionFile(("./api/SessionManager/sess_" + hash).c_str());

		std::getline(sessionFile, line);
		session.name = line;
		std::getline(sessionFile, line);
		session.about = line;
		sessionFile.close();
		return session;
	};
};

#endif