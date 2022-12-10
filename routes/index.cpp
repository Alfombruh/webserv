#include "routes.h"


static void get(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).html("./public/html/index.html").send();
};

static void post(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_405).send();
	// if(req.getBody().empty())
	// {
	// 		return;
	// }
	// res.status(STATUS_200).text("has llegado a index.post").send();
};

static void delet(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_200).text("has llegado a index.delete").send();
};

static void getFavicon(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_300).img("./public/images/42-Urduliz-Logo-web.png").send();
};

static void direct1(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_301).redirect("/YoupiBanane").send();
};
static void youpiDirect2(Request &req, Response &res)
{
	if (req.getMethod() == GET)
	{
		(void)req;
		res.status(STATUS_301).redirect("/YoupiBanane" + req.getRoute()).send();
	}
	else
	{
		std::vector<char*> cstrings;
		cstrings.reserve(req.env.env.size());
		for(size_t i = 0; i < req.env.env.size(); ++i) {
			cstrings.push_back(const_cast<char*>(req.env.env[i].c_str()));
		}
		cstrings.push_back(NULL);
		res.setBody(req.getBody());
		res.status(STATUS_200).text_python("/cgi-bin/ubuntu_cgi_tester", &cstrings[0]).send();
	}
};
static void nopDirect2(Request &req, Response &res)
{
	(void)req;
	res.status(STATUS_301).redirect("/Youpibanane/nop" + req.getRoute()).send();
};

static bool useNop(Router &router)
{
	if (router.getReqRoute() == "/"){
		if (router.get("/", &get)){
			return true;
		}
	}
	if (router.get("/other.pouic", &get))
		return true;
	if (router.get("/youpi.bad_extension", &get))
		return true;
	return false;
};

static bool useYeah(Router &router)
{
	if (router.getReqRoute() == "/")
		return false;
	if (router.get("/not_happy.bad_extension", &get))
		return true;
	return false;
};

static bool yupi(Router &router)
{
	if (router.get("/", &get))
		return true;
	if (router.get("/youpi.bad_extension", &get))
		return true;
	if (router.get("/youpi.bla", &get))
		return true;
	if (router.use("/nop", &useNop))
		return true;
	if (router.use("/Yeah", &useYeah))
		return true;
	return router.notFound();
};

static bool useDirect(Router &router)
{
	router.create_env(&parse_env);
	if (router.getReqRoute() == "/"){
		if (router.get("/", &direct1)){
			return true;
		}
	}
	else if (router.post(router.getReqRoute(), &youpiDirect2))
	{
		return true;
	}
	else if (router.get(router.getReqRoute(), &youpiDirect2))
	{
		return true;
	}
	return router.notFound();
};


bool index(Router &router)
{
	if (router.use("/public", &publicFolder))
		return true;
	if (router.use("/galery", &galery))
		return true;
	if (router.use("/login", &login))
		return true;
	if (router.get("/favicon.ico", &getFavicon))
		return true;
	if (router.use("/cgi-bin", &cgi))
		return true;
	if (router.use("/directory", &useDirect))
		return true;
	if (router.use("/YoupiBanane", &yupi))
		return true;
	if (router.get("/", &get))
		return true;
	if (router.post("/", &post))
		return true;
	if (router.delet("/", &delet))
		return true;
	return router.notFound();
};