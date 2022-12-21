#include "routes.h"
#include "../api/routes/api_routes.h"

bool location(Router &router, Location &location)
{
	if (location.api.empty() == false)
		return apiIndex(router);
	if (router.get(location, &get))
		return true;
	if (router.post(location, &post))
		return true;
	if (router.delet(location, &delet))
		return true;
	router.notFound();
	return true;
};