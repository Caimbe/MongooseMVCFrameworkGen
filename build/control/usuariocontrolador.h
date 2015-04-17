#ifndef USUARIOCONTROLADOR_H
#define USUARIOCONTROLADOR_H

#include "controlador.h"

class UsuarioControlador
{
public:
	UsuarioControlador();
	void setup();
	void list(Request &request, StreamResponse &response);
};

#endif