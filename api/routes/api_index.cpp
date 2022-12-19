#include "api_routes.h"

bool apiIndex(Router &router)
{
	if (router.use("/login", &login))
		return true;
	return router.notFound();
};