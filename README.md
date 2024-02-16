# obstacle_avoiding_robot

using a pi pico programmed in c++ using the arduino ide
tank style chassis with a ultrasonic sensor mounted to a servo

```
while true
  drive forward until obstacle is detected
  sweep servo left and right recording the distance
  turn towards the side with the greatest distance
```
