// Configuraci�n de lo que va a usar el proyecto
//#define PracticasRigidos
//#define P4
#define Proyecto
//#define CamaraTeclas
//#define Explicaciones


// Configuraci�n del render y la ventana
#ifdef Proyecto
	#define INIT_WINDOW_W 1280
	#define INIT_WINDOW_H 720
#else
	#define INIT_WINDOW_W 512
	#define INIT_WINDOW_H 512
#endif
