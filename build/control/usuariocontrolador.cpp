#include "usuariocontrolador.h"
#include <htmlcxx/html/Page.h>
using namespace htmlcxx::HTML;

UsuarioControlador::UsuarioControlador()
{
}

void UsuarioControlador::setup()
{
	addRoute("GET", "/usuario/list", UsuarioControlador, list);
}
void UsuarioControlador::list(Request &request, StreamResponse &response)
{
	Page page = createPage("/usuario/list.html");
	response << page;
}
