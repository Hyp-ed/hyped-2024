import { env } from '@hyped/env';
import { Module } from '@nestjs/common';
import { MqttModule } from 'nest-mqtt';

const mqttClient = MqttModule.forRoot({
  host: env.PUBLIC_MQTT_BROKER_HOST,
});

@Module({
  imports: [mqttClient],
  exports: [mqttClient],
})
export class MqttClientModule {}
