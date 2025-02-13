typedef struct
{
    int i;
    int increment;
} counter_t;

counter_t *init(int start_value, int increment);

void destroy(counter_t *counter);

void count(counter_t *counter);
