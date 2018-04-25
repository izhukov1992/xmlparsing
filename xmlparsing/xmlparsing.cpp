// xmlparsing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
//#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

xmlNode* findNodeByName(xmlNode* rootnode, const xmlChar* nodename)
{
    xmlNode* node = rootnode;

    while(node != NULL){

        if(!xmlStrcmp(node->name, nodename)){
            break; 
        }
		else if (node->children != NULL) {
			xmlNode* intNode =  findNodeByName(node->children, nodename); 
			if(intNode != NULL) {
				node = intNode;
				break;
			}
		}
        node = node->next;
    }

    return node;
}

/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o tree2 tree2.c
 */

/* A simple example how to create DOM. Libxml2 automagically 
 * allocates the necessary amount of memory to it.
*/
int
main(int argc, char **argv)
{
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    char buff[256];
    int i, j;

    LIBXML_TEST_VERSION;

    /* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);

    /*
     * Creates a DTD declaration. Isn't mandatory. 
     */
    xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

    /* 
     * xmlNewChild() creates a new node, which is "attached" as child node
     * of root_node node. 
     */
    xmlNewChild(root_node, NULL, BAD_CAST "node1",
                BAD_CAST "content of node 1");
    /* 
     * The same as above, but the new child node doesn't have a content 
     */
    xmlNewChild(root_node, NULL, BAD_CAST "node2", NULL);

    /* 
     * xmlNewProp() creates attributes, which is "attached" to an node.
     * It returns xmlAttrPtr, which isn't used here.
     */
    node =
        xmlNewChild(root_node, NULL, BAD_CAST "node3",
                    BAD_CAST "this node has attributes");
    xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
    xmlNewProp(node, BAD_CAST "foo", BAD_CAST "bar");

    /*
     * Here goes another way to create nodes. xmlNewNode() and xmlNewText
     * creates a node and a text node separately. They are "attached"
     * by xmlAddChild() 
     */
    node = xmlNewNode(NULL, BAD_CAST "node4");
    node1 = xmlNewText(BAD_CAST
                   "other way to create content (which is also a node)");
    xmlAddChild(node, node1);
    xmlAddChild(root_node, node);

    /* 
     * A simple loop that "automates" nodes creation 
     */
    for (i = 5; i < 7; i++) {
        sprintf(buff, "node%d", i);
        node = xmlNewChild(root_node, NULL, BAD_CAST buff, NULL);
        for (j = 1; j < 4; j++) {
            sprintf(buff, "node%d%d", i, j);
            node1 = xmlNewChild(node, NULL, BAD_CAST buff, NULL);
            xmlNewProp(node1, BAD_CAST "odd", BAD_CAST((j % 2) ? "no" : "yes"));
        }
    }
	
    /*
     * Dump the document to a buffer and print it
     * for demonstration purposes.
     */
    int buffersize;
    xmlChar* xmlbuff;

    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    printf("%s", (char *) xmlbuff);

	/*
	 * Read and parse doc
	 * read content
	 */
	xmlDoc* doc2 = xmlParseDoc(xmlbuff);
	//xmlReadMemory((const char*)xmlbuff, strlen((const char*)xmlbuff), NULL, NULL, 0);

	xmlNode* r_node = xmlDocGetRootElement(doc2);
	xmlNode* f_node = findNodeByName(r_node, (const xmlChar*)("node3"));

	if (f_node != NULL)
	{
		printf("%s", (char *)f_node->name);
		printf("%s", (char *)xmlNodeGetContent(f_node));
	}

    /*
     * Free associated memory.
     */
    xmlFree(xmlbuff);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
#else
int main(void) {
    fprintf(stderr, "tree support not compiled in\n");
    exit(1);
}
#endif
