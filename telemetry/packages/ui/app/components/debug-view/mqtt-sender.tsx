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

const formSchema = z.object({
  topic: z.string(),
  message: z.string(),
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
    },
  });

  function onSubmit(values: z.infer<typeof formSchema>) {
    if (!publish) {
      toast.error('Could not publish message.');
      return;
    }
    // TODO: currently not working...
    publish(
      values.topic,
      values.message,
      {
        qos: 0,
        retain: false,
      },
      (error) => {
        if (error) {
          log(`MQTT publish error: ${error}`);
        }
      },
    );
  }

  return (
    <Form {...form}>
      <form onSubmit={form.handleSubmit(onSubmit)} className="space-y-8">
        <FormField
          control={form.control}
          name="topic"
          render={({ field }) => (
            <FormItem className="flex gap-4 items-center">
              <FormLabel>Topic:</FormLabel>
              <FormControl>
                <Input {...field} />
              </FormControl>
              {/* <FormDescription>
                This is your public display name.
              </FormDescription> */}
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
              {/* <FormDescription>
                This is your public display name.
              </FormDescription> */}
              <FormMessage />
            </FormItem>
          )}
        />
        <div className="flex justify-end">
          <Button type="submit">Send</Button>
        </div>
      </form>
    </Form>
  );
};
