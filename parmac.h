#ifndef PARMAC_H
#define PARMAC_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

struct parmac;
struct parmac_state;

typedef const char *(*parmac_event)(const char *src,void *userdata);

typedef void (*parmac_machine)(struct parmac *p);

typedef void (*parmac_state_enter)(unsigned int stkDepth,
                                      const char *machineName,
                                      const char *fromStateName,
                                      const char *toStateName,
                                      const char *parseStart,
                                      const char *parseEnd,
                                      void *userdata);

typedef void (*parmac_state_leave)(unsigned int stkDepth,
                                   const char *machineName,
                                   const char *fromStateName,
                                   const char *toStateName,
                                   void *userdata);

struct parmac_state {
  const char *name;
  parmac_state_enter enter;
  parmac_state_leave leave;
};

struct parmac_transition {
  const struct parmac_state *fromState,*toState;
  parmac_event event;
  parmac_machine machine;
};

struct parmac {
  const char *name;
  unsigned int pos;
  const struct parmac_transition *trsn,*trsnStart,*trsnEnd;
  const struct parmac_state *state,*startState,*endState;
};

#ifdef __cplusplus
extern "C" {
#endif

  struct parmac *parmac_set(struct parmac *p,
                            const char *name,
                            const struct parmac_state *startState,
                            const struct parmac_state *endState,
                            const struct parmac_transition *startTrsn,
                            const struct parmac_transition *endTrsn);

  bool parmac_run(struct parmac *stk,
                  unsigned int *pStkDepth,
                  const char *src,void *userdata);

  bool parmac_failed(struct parmac *stk);
  const char *parmac_last_src(struct parmac *stk,unsigned int stkDepth,const char *src);

#ifdef __cplusplus
}
#endif

#endif
