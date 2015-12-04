/***************************************************************************
 * groovy.c
 * Regex callback based parser for groovy syntax
 **************************************************************************/
/* INCLUDE FILES */
#include "general.h"

#include "routines.h"
#include "parse.h"
#include "read.h"

#define COMMONK_UNDEFINED -1

/* DATA DEFINITIONS */
typedef enum eGroovyKinds {
	GK_UNDEFINED = COMMONK_UNDEFINED,
	GK_CLASS, GK_ENUM_CONSTANT, GK_FIELD, GK_ENUM, GK_INTERFACE,
	GK_LOCAL, GK_METHOD, GK_PACKAGE, GK_ACCESS, GK_CLASS_PREFIX
} groovyKind;

static kindOption GroovyKinds [] = {
	{ TRUE,  'c', "class",         "classes"},
	{ TRUE,  'e', "enumConstant",  "enum constants"},
	{ TRUE,  'f', "field",         "fields"},
	{ TRUE,  'g', "enum",          "enum types"},
	{ TRUE,  'i', "interface",     "interfaces"},
	{ FALSE, 'l', "local",         "local variables"},
	{ TRUE,  'm', "method",        "methods"},
	{ TRUE,  'p', "package",       "packages"},
};

/* FUNCTION DEFINITIONS */

static void definition (const char *const line, const regexMatch *const matches, const unsigned int count)
{
	if (count > 1)    /* should always be true per regex */
	{
		/*vString *const name = vStringNew ();*/
		/*vStringNCopyS (name, line + matches [1].start, matches [1].length);*/
		/*makeSimpleTag (name, GroovyKinds, K_DEFINE);*/
	}
}


static void g_method (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_field (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_property (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_class (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_enum (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_trait (const char *const line, const regexMatch *const matches, const unsigned int count) {}
static void g_interface (const char *const line, const regexMatch *const matches, const unsigned int count) {}


static void findGroovyTags (void)
{
	while (readLineFromInputFile() != NULL)
		;  /* don't need to do anything here since callback is sufficient */
}

static void installGroovy (const langType language)
{
	addCallbackRegex (language, "^def[ \t]+([a-zA-Z0-9_]+)", NULL, definition);
	addCallbackRegex (language, "^[ \t]*[(private|public|protected|final|synchronized|abstract)[ \t]*]*(def|[A-Za-z0-9_.]+)[ \t]+([A-Za-z0-9_\"]+)[ \t]*\(", NULL, g_method);
	addCallbackRegex (language, "^[ \t]*private[ \t]+(final)?[ \t]*(def|[a-zA-Z0-9_.]+)[ \t]+([A-Za-z0-9_]+)($|[ \t]+=)", NULL, g_field);
	addCallbackRegex (language, "^[ \t]*(public)?(final)?[ \t]*(def|[a-zA-Z0-9_.]+)[ \t]+([A-Za-z0-9_]+)($|[ \t]+=)", NULL, g_property);
	addCallbackRegex (language, "^[ \t]*[(private|public|abstract|static)[ \t]*]*class[ \t]+([A-Za-z0-9_]+)[ \t]*", NULL, g_class);
	addCallbackRegex (language, "^[ \t]*[(private|public|abstract|static)[ \t]*]*enum[ \t]+([A-Za-z0-9_]+)[ \t]*", NULL, g_enum);
	addCallbackRegex (language, "^[ \t]*[(private|public|abstract|static)[ \t]*]*trait[ \t]+([A-Za-z0-9_]+)[ \t]*", NULL, g_trait);
	addCallbackRegex (language, "^[ \t]*[(private|public|abstract|static)[ \t]*]*interface[ \t]+([A-Za-z0-9_]+)[ \t]*", NULL, g_interface);
}

/* Create parser definition stucture */
extern parserDefinition* GroovyParser (void)
{
	static const char *const extensions [] = { "groovy", NULL };
	parserDefinition* def = parserNew ("Groovy");
	def->kinds      = GroovyKinds;
	def->kindCount  = ARRAY_SIZE (GroovyKinds);
	def->extensions = extensions;
	def->parser     = findGroovyTags;
	def->initialize = installGroovy;
	return def;
}
