#include "api_routes.h"

bool apiIndex(Router &router)
{
	if (router.apiUse("/login", &login))
		return true;
	return router.notFound();
};