import { useForm } from 'react-hook-form';
import * as z from 'zod';
import { zodResolver } from '@hookform/resolvers/zod';
import { Button } from '@/components/ui/button';
import {
  Form,
  FormControl,
  FormDescription,
  FormField,
  FormItem,
  FormLabel,
  FormMessage,
} from '@/components/ui/form';
import { Input } from '@/components/ui/input';
import { Textarea } from '../ui/textarea';
import { useMQTT } from '@/context/mqtt';
import toast from 'react-hot-toast';
import { log } from '@/lib/logger';
import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { Send } from 'lucide-react';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';

const formSchema = z.object({
  topic: z.string().min(1, {
    message: 'Topic must not be empty',
  }),
  message: z.string(),
  qos: z.coerce
    .number()
    .optional()
    .refine((val) => (val ? [0, 1, 2].includes(val) : true), {
      message: 'QoS must be 0, 1, or 2',
    }),
  retain: z.coerce.boolean().optional(),
});

/**
 * Custom MQTT message sender to allow sending of arbitrary MQTT messages.
 * @returns The MQTT sender
 */
export const MqttSender = () => {
  const { customPublish: publish } = useMQTT();

  const form = useForm<z.infer<typeof formSchema>>({
    resolver: zodResolver(formSchema),
    defaultValues: {
      topic: '',
      message: '',
      qos: 0,
      retain: false,
    },
  });

  function onSubmit(values: z.infer<typeof formSchema>) {
    if (!publish) {
      toast.error('Could not publish message.');
      return;
    }
    publish(values.topic, values.message, {
      qos: values.qos as 0 | 1 | 2,
      retain: values.retain,
    });
    log(`Published message to ${values.topic}: ${values.message}`);
    toast.success('Message published.');
    // Reset form
    form.reset();
  }

  return (
    <Form {...form}>
      <form onSubmit={form.handleSubmit(onSubmit)}>
        <Card className="border-none">
          <CardHeader>
            <CardTitle>MQTT</CardTitle>
            <CardDescription>Send arbitrary MQTT messages</CardDescription>
          </CardHeader>
          <CardContent className="space-y-8">
            <FormField
              control={form.control}
              name="topic"
              render={({ field }) => (
                <FormItem className="flex gap-4 items-center">
                  <FormLabel>Topic:</FormLabel>
                  <FormControl>
                    <Input {...field} />
                  </FormControl>
                  <FormMessage />
                </FormItem>
              )}
            />
            <FormField
              control={form.control}
              name="message"
              render={({ field }) => (
                <FormItem>
                  <FormLabel>Message:</FormLabel>
                  <FormControl>
                    <Textarea {...field} />
                  </FormControl>
                  <FormMessage />
                </FormItem>
              )}
            />
          </CardContent>
          <CardFooter>
            <div className="w-full flex justify-between gap-4">
              <FormField
                control={form.control}
                name="qos"
                render={({ field }) => (
                  <FormItem className="flex gap-2 items-center text-sm space-y-0">
                    <FormLabel>QoS:</FormLabel>
                    <Select
                      onValueChange={field.onChange}
                      defaultValue={field.value?.toString()}
                    >
                      <FormControl>
                        <SelectTrigger>
                          <SelectValue />
                        </SelectTrigger>
                      </FormControl>
                      <SelectContent>
                        <SelectItem value="0">0 (At most once)</SelectItem>
                        <SelectItem value="1">1 (At least once)</SelectItem>
                        <SelectItem value="2">2 (Exactly once)</SelectItem>
                      </SelectContent>
                    </Select>
                    <FormMessage />
                  </FormItem>
                )}
              />
              <FormField
                control={form.control}
                name="retain"
                render={({ field }) => (
                  <FormItem className="flex gap-2 items-center text-sm space-y-0">
                    <FormLabel>Retain:</FormLabel>
                    <Select
                      onValueChange={field.onChange}
                      defaultValue={field.value?.toString()}
                    >
                      <FormControl>
                        <SelectTrigger>
                          <SelectValue />
                        </SelectTrigger>
                      </FormControl>
                      <SelectContent>
                        <SelectItem value="true">true</SelectItem>
                        <SelectItem value="false">false</SelectItem>
                      </SelectContent>
                    </Select>
                    <FormMessage />
                  </FormItem>
                )}
              />
              <Button type="submit" className="flex gap-2">
                Send <Send size={16} />
              </Button>
            </div>
          </CardFooter>
        </Card>
      </form>
    </Form>
  );
};
