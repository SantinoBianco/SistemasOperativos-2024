#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "strextra.h"
#include "command.h"
#include <string.h>
#include <glib.h>


struct scommand_s{
    GSList* list;
    char* input;
    char* output;
};

scommand scommand_new(void){
    scommand res = malloc(sizeof(struct scommand_s));
    
    res->list = NULL;
    res->input = NULL;
    res->output = NULL;

    assert(res != NULL && scommand_is_empty (res) && scommand_get_redir_in (res) == NULL && 
    scommand_get_redir_out (res) == NULL);
    return res;
}

scommand scommand_destroy(scommand self){
    assert(self != NULL);

    g_slist_free_full(self->list, free);
    free(self->input); self->input = NULL;
    free(self->output); self->output = NULL;
    free(self); self = NULL;

    assert(self == NULL);
    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self != NULL && argument != NULL);

    self->list = g_slist_append(self->list, argument);

    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));

    GSList *tmp = self->list;
    self->list = self->list->next;
    g_slist_free_1(tmp);
}

void scommand_set_redir_in(scommand self, char * filename){
    assert(self!=NULL);

    if(self->input != NULL){
        free(self->input);
    }
    self->input = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self!=NULL);

    if(self->output != NULL){
        free(self->output);
    }
    self->output = filename;
}

bool scommand_is_empty(const scommand self){
    assert(self != NULL);
    return(g_slist_length(self->list)==0);
}

unsigned int scommand_length(const scommand self){
    assert(self!=NULL);

    return(g_slist_length(self->list));
}

char * scommand_front(const scommand self){
    assert(self!=NULL && !scommand_is_empty(self));

    char *result = self->list->data; 

    assert(result!=NULL);
    return result;
}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);

    char *result = self->input;

    return result;
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);

    char *result = self->output;

    return result;
}

char * scommand_to_string(const scommand self){
    assert(self!=NULL);
    char *result = strdup("");
    GSList *list_c = self->list;

    while(list_c != NULL){
        char *tmp_str = strmerge(result, list_c->data);
        free(result);
        result = strmerge(tmp_str, " ");
        free(tmp_str);
        list_c = list_c->next;
    }

    if(self->list != NULL && strlen(result) > 0){
        result[strlen(result) - 1] = '\0'; //delete last " "
    }

    if(self->input != NULL){
        char *tmp_str = strmerge(result, " < ");
        free(result);
        result = strmerge(tmp_str, scommand_get_redir_in(self));
        free(tmp_str);
    }

    if(self->output != NULL){
        char *tmp_str = strmerge(result, " > ");
        free(result);
        result = strmerge(tmp_str, scommand_get_redir_out(self));
        free(tmp_str);
    }

    assert(scommand_is_empty(self) || scommand_get_redir_in(self)==NULL || scommand_get_redir_out(self)==NULL ||
    strlen(result)>0);
    return result;
}

struct pipeline_s{
    GSList* command;
    bool foreground; //true:foreground, false:background
};

pipeline pipeline_new(void){
    pipeline res = malloc(sizeof(struct pipeline_s));
    res->command = NULL;
    res->foreground = true;

    assert(res != NULL && pipeline_is_empty(res) && pipeline_get_wait(res));
    return res;
}

pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);

    g_slist_free_full(self->command, g_free);
    free(self);
    self = NULL;

    assert(self == NULL);
    return self;
}

void pipeline_push_back(pipeline self, scommand sc){
    assert(self != NULL && sc != NULL);

    self->command = g_slist_append(self->command, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));

    free(self->command->data);  
    self->command = g_slist_remove(self->command, self->command->data);
}

void pipeline_set_wait(pipeline self, const bool w){
    assert(self!=NULL);

    self->foreground = w;
}

bool pipeline_is_empty(const pipeline self){
    assert(self!=NULL);

    return(g_slist_length(self->command)==0);
}

unsigned int pipeline_length(const pipeline self){
    assert(self!=NULL);

    return(g_slist_length(self->command));
}

scommand pipeline_front(const pipeline self){
    assert(self!=NULL && !pipeline_is_empty(self));

    scommand result = self->command->data;

    assert(result!=NULL);
    return result;
}

bool pipeline_get_wait(const pipeline self){
    assert(self!=NULL);

    return(self->foreground);
}

char * pipeline_to_string(const pipeline self) {
    assert(self!=NULL);
    char *result = strdup("");
    GSList *command_c = self->command;

    while(command_c != NULL){
        scommand cmd = (scommand)command_c->data;
        char *str_cmd = scommand_to_string(cmd);
        char *tmp = strmerge(result, str_cmd);
        free(result);
        result = strmerge(tmp, " | ");
        free(tmp);
        free(str_cmd);
        command_c = command_c->next;
    }

    size_t len = strlen(result);
    if (len >= 3 && strcmp(result + len - 3, " | ") == 0) {
        result[len - 3] = '\0';
    }

    if(!self->foreground){
        result = strmerge(result, " &");
    }

    assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(result)>0);
    return result;
}