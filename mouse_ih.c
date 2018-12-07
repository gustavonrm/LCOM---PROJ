#include <lcom/lcf.h>

#include "mouse_ih.h"
#include "i8042.h"

int mouse_hook_id = 2;
uint8_t bit_no = 0;
uint8_t pack;
uint8_t packets[3];

unsigned int count = 0;

States state;

//projeto
bool lmb;
bool rmb;
bool mmb;
//projeto
uint8_t trajx;
uint8_t trajy;

///////////////////////

int subscribe_mouse(uint8_t *bit_no)
{
  *bit_no = mouse_hook_id;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK)
    return 1; //sets exclusive interrupts to mouse_hook_id
  else
    return 0;
}

int unsubscribe_mouse()
{
  if (sys_irqrmpolicy(&mouse_hook_id) != OK)
    return 1;
  else
    return 0;
}

int enable_stream()
{
  uint32_t stat;

  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  if ((stat & IBF) == 0)
  { //Check if IN_BUF is ready
    if (sys_outb(KBC_CMD_REG, MOUSE_CMD) != OK)
      return 1; //Write cmd in cmg_reg saying I'll write to mouse next
  }

  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  if ((stat & IBF) == 0)
  { //Check if IN_BUF is ready
    if (sys_outb(IN_BUF, ENABLE_STREAM_CMD) != OK)
      return 1; //write mouse cmd
  }

  //if(sys_inb(STAT_REG,&stat) != OK) return 1;
  //if((stat & OBF) == 0){ //Check if OUT_BUF is ready
  if (sys_inb(OUT_BUF, &stat) != OK)
    return 1; //Read Mouse acknowledgement
  uint8_t st = (uint8_t)stat;
  if (st == ACK)
    return 0; //OKAY
  else if (st == NACK)
    return 1; //tries again
  else if (st == ERROR)
    return -1;
  //}

  return 0;
}

int disable_stream()
{
  uint32_t stat;

  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  if ((stat & IBF) == 0)
  { //Check if IN_BUF is ready
    if (sys_outb(KBC_CMD_REG, MOUSE_CMD) != OK)
      return 1; //Write cmd in cmg_reg saying I'll write to mouse next
  }

  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  if ((stat & IBF) == 0)
  { //Check if IN_BUF is ready
    if (sys_outb(IN_BUF, DISABLE_STREAM_CMD) != OK)
      return 1; //write mouse cmd
  }

  //if(sys_inb(STAT_REG,&stat) != OK) return 1;
  //if((stat & OBF) == 0){ //Check if OUT_BUF is ready
  if (sys_inb(OUT_BUF, &stat) != OK)
    return 1; //Read Mouse acknowledgement
  uint8_t st = (uint8_t)stat;
  if (st == ACK)
    return 0; //OKAY
  else if (st == NACK)
    return 1; //tries again
  else if (st == ERROR)
    return -1;
  //}

  return 0;
}

void read_data()
{
  uint32_t stat, data;
  if (sys_inb(STAT_REG, &stat) != OK)
    return; //Read status
  if ((stat & OBF) && (stat & AUX))
  { //Check if there's anything in OUTPUT BUFFER
    if (sys_inb(OUT_BUF, &data) != OK)
      return;
    pack = (uint8_t)data;
  }
  else{}
}

void(mouse_ih)()
{
  uint32_t stat, data;
  if (sys_inb(STAT_REG, &stat) != OK)return;
  if (stat & OBF)
  { //Check if there's anything in OUTPUT BUFFER
    if (sys_inb(OUT_BUF, &data) != OK)return;
      pack = (uint8_t)data;
  }
}

void parse_packet(struct packet *pp, uint8_t packets[3])
{
  memcpy(pp->bytes, packets, sizeof(pp->bytes));
  pp->rb = (packets[0] & BIT(1));
  pp->lb = (packets[0] & BIT(0));
  pp->mb = (packets[0] & BIT(2));
  pp->x_ov = (packets[0] & BIT(6));
  pp->y_ov = (packets[0] & BIT(7));
  if (packets[0] & BIT(4))
  {
    pp->delta_x = (0xFF00 | (uint16_t)packets[1]);
  }
  else
    pp->delta_x = (uint16_t)packets[1];

  if (packets[0] & BIT(5))
  {
    pp->delta_y = (0xFF00 | (uint16_t)packets[2]);
  }
  else
    pp->delta_y = (uint16_t)packets[2];
}

int(mouse_write)(uint32_t CMD)
{
  uint32_t stat;

  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;

  if ((stat & IBF) == 0)
  { //Check if IN_BUF is ready
    if (sys_outb(KBC_CMD_REG, MOUSE_CMD) != OK)
      return 1; //Write cmd in cmg_reg saying I'll write to mouse next
  }
  if (sys_inb(STAT_REG, &stat) != OK)
    return 1;
  if ((stat & IBF) == 0)
  {
    if (sys_outb(IN_BUF, CMD) != OK)
      return 1; //write mouse cmd
  }
  else
    printf("\nERROR IN MOUSE_WRITE");

  //if((stat & OBF) == 0){ //Check if OUT_BUF is ready
  if (sys_inb(OUT_BUF, &stat) != OK)
    return 1; //Read Mouse acknowledgement
  uint8_t st = (uint8_t)stat;
  if (st == ACK)
    return 0; //OKAY
  else if (st == NACK)
    return 1; //tries again
  else if (st == ERROR)
    return -1;
  //}
  return 0; //if success
}

int read_pcktB()
{ //function similir to all oothers implemented and those on lab3, it reads the byte basically
  read_data();
  if ((pack & BIT(3)) == 0)
    return 1;
  packets[0] = pack;

  read_data();
  packets[1] = pack;

  read_data();
  packets[2] = pack;

  return 0;
}

//global
bool previous_LP;
bool previous_RP;
int16_t x = 0, y = 0;

struct mouse_ev *update_event_type(struct packet *pp)
{

  struct mouse_ev *events = malloc(sizeof *events);

  //define types
  if ((pp->delta_x != 0 || pp->delta_y != 0))
  {
    events->type = MOUSE_MOV;
    events->delta_x = (pp->delta_x) + x;
    events->delta_y = (pp->delta_y) + y;
    //printf("move\n");
    return events;
  }

  if (pp->lb == true)
  {
    events->type = LB_PRESSED;
    previous_LP = true;
    //printf("lbp\n");
    return events;
  }
  if (pp->rb == true)
  {
    events->type = RB_PRESSED;
    previous_RP = true;
    //printf("rbp\n");
    return events;
  }

  if (pp->lb == false)
  {
    if (previous_LP == true)
    {
      events->type = LB_RELEASED;
      previous_LP = false;
      //printf("lbr\n");
      return events;
    }
  }

  if (pp->rb == false)
  {
    if (previous_RP == true)
    {
      events->type = RB_RELEASED;
      previous_RP = false;
      //printf("rbr\n");
      return events;
    }
  }

  if (pp->mb == true)
  {
    events->type = BUTTON_EV;
    //printf("BE\n");
    return events;
  }

  x += (pp->delta_x);
  y += (pp->delta_y);

  return events;
}

//////////////////////////
//////////////////////////
//Project

int mouse_cursor(/*int *x, int *y,*/ bool *LB)
{

  message msg;
  int ipc_status, r;
  int counter = 0;
  uint8_t packets[3];
  int cnt = 1;
  while (cnt != 0)
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & BIT(bit_no))
        { /* subscribed interrupt */
          mouse_ih();
          if (counter == 0)
          { //Expecting first packet
            if (pack & BIT(3))
            { //It's first packet
              packets[0] = pack;
              counter++;
            }
          }
          else
          { //If expecting 2º or 3º packets
            packets[counter] = pack;
            counter++;
          }
          if (counter == 3)
          { //If I have all three bytes
            counter = 0;
            cnt--;
            if ((packets[0] & BIT(0)) == 1)
            {
              *LB = true;
            }
            if ((packets[0] & BIT(0)) == 0)
            {
              *LB = false;
            }
            /*if (packets[0] & BIT(4))
            {
              x = ((int)(0xFF00 | (uint16_t)packets[1]));
            }
            else
              x = (int)packets[1];

            if (packets[0] & BIT(5))
            {
              y= ((int)(0xFF00 | (uint16_t)packets[2]));
            }
            else
              y= (int)packets[2];*/
          }
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    }
  }
  return 0;
}
