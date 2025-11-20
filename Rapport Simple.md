## Modifications du schéma
Nous avons opéré plusieurs modifications au schéma donné dans le sujet :
- Introduction d'une structure `FIFO` englobant les `processus`
- Toutes les fonctions ont été renommées pour plus de clarté
### Fonctions Processus
#### Init et Free

```
t_processus* creer_processus(int pid, int arrivee, int duree);
void free_processus(t_processus* p);
```
deviennent
```
t_processus *processus_init(int pid, int arrivee, int duree);
void processus_free(t_processus **process);
void processus_free_recursive(t_processus *process)
```
>`processus_free` prend maintenant un `t_processus **`, pour pouvoir free l'espace mémoire du processus, et ensuite set son pointeur à `NULL`
>On a également ajouté `processus_free_recursive` qui va free tous les processus de la chaine

#### Load

```
t_processus* charger_processus(char* nom_fichier, int* nb_processus);
```
devient
```
t_processus *processus_load(char *nom_fichier, int *processes_loaded);
```
>`processus_load` lit des lignes jusque le EOF, et renvoie le nombre de processus chargés dans `int *processes_loaded`

#### Fonctions Bonus
```
void processus_print(t_processus *process);
```
>Permet un affichage formaté de la chaine de processus dans le terminal

### Fonctions FIFO
#### Structures
```
typedef struct s_FIFO
{
    t_processus *first; // élément en tête de la queue
    t_processus *last;  // dernier élément de la queue
    int size;           // taille de la queue
} FIFO;
```
>On opte pour l'ajout d'une structure qui encapsule une chaine process. Cela permet  d'avoir par la suite un cout d'enfilage et de défilage en O de 1 

```
typedef enum PROCESSFIELDS
{
    PID,
    ARRIVEE,
    DUREE
} PROCESSFIELDS;
```
>On ajoute également une Enum correspondant aux attributs de `t_processus`, qui sera utilisée dans les fonction de tri pour pouvoir indiquer l'attribut en fonction duquel on trie les processus
#### Init et Clear
```
t_processus* fifo_init ();
t_processus* fifo_clear (t_processus* file);
```
devient
```
FIFO *fifo_init();
FIFO *fifo_init_from_process(t_processus *process);
FIFO *fifo_init_sorted_from_process(t_processus *process, PROCESSFIELDS field);
void fifo_free(FIFO **queue);
```
>Nous avons ajouter 2 fonctions permettant l'initialisation d'une FIFO à partir d'une chaine de processus existante : 
>- Une les ajoutant dans l'ordre existant
>- L'autre les ajoutant et les triant en fonction d'un attribut donné `PROCESSFIELDS field`
>La fonction `fifo_free` prend également un `FIFO **` pour pouvoir définir le pointeur de la FIFO à détruire à NULL

#### Taille
```
int fifo_vide (t_processus* file);
```
devient
```
int fifo_is_empty(FIFO *queue);
```
>Grace à la structure FIFO, cette fonction devient triviale

#### Empilage et dépilage
```
t_processus* fifo_enfiler (t_processus* file, t_processus* p);
t_processus* fifo_defiler (t_processus** file);
```
devient
```
void fifo_add(FIFO *queue, t_processus *process);
void fifo_add_sorted(FIFO *queue, t_processus *process, PROCESSFIELDS field);
t_processus *fifo_unqueue(FIFO *queue);
```
>On choisi de réutiliser les FIFO dans l'implémentation de listes triées, ce qui veut dire que si on insère une chaine de processus dans une FIFO en utilisant uniquement la fonction `fifo_add_sorted` avec le même `PROCESSFIELDS field`, on se retrouve avec une FIFO triée par l'attribut spécifié dans `field`.
>Nous n'avons pas implémenté de tri 'descendant', car il n'est pas utilisé dans le sujet (donc lorsque référence est faite à une liste triée, cette liste est triée de manière 'ascendante')

#### Fonctions bonus
```
void fifo_print(FIFO *queue);
int fifo_is_sorted(FIFO *queue, PROCESSFIELDS field);
```
>On ajoute une manière de pouvoir visualiser les FIFO de manière formatée
>On ajoute également une fonction pour vérifier si une liste est triée selon un attribut `PROCESSFIELDS field` particulier.
### Fonctions de simulation
```
void simuler_fcfs(t_processus* tableau, int nb_processus);
void simuler_sjf(t_processus* tableau, int nb_processus);
```
devient
```
void simuler_fcfs(FIFO *tab);
void simuler_sjf(FIFO *tab);
```
>Peu de modifications ici, hormis que grâce à la structure FIFO, il n'est pas nécessaire d'indiquer le nombre de processus, il est trouvé dans la FIFO.