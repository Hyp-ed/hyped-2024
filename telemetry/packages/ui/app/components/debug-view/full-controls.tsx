import React from 'react';
import { Button } from '../ui/button';
import { cn } from '@/lib/utils';
import {
  Card,
  CardContent,
  CardDescription,
  CardFooter,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';

export const FullControls = () => {
  return (
    <Card className="w-full border-none">
      <CardHeader>
        <CardTitle>Controls</CardTitle>
        <CardDescription>Granular pod controls</CardDescription>
      </CardHeader>
      <CardContent>
        <div className="flex gap-4 flex-wrap">
          <div className="space-y-2">
            <ButtonLabel>Start/stop</ButtonLabel>
            <ButtonPair>
              <LeftButton>Start</LeftButton>
              <RightButton>Stop</RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Active suspension</ButtonLabel>
            <ButtonPair>
              <LeftButton>Raise</LeftButton>
              <RightButton>Lower</RightButton>
            </ButtonPair>
            <ControlButton>Tilt</ControlButton>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Friction brakes</ButtonLabel>
            <ButtonPair>
              <LeftButton>Clamp</LeftButton>
              <RightButton>Retract</RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>High power</ButtonLabel>
            <ButtonPair>
              <LeftButton>Start HP</LeftButton>
              <RightButton>Stop HP</RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Levitation</ButtonLabel>
            <ControlButton>Levitate</ControlButton>
          </div>
        </div>
      </CardContent>
    </Card>
  );
};

const ButtonLabel = ({ children }: { children: React.ReactNode }) => (
  <p className="text-sm font-bold">{children}</p>
);

const ButtonPair = ({ children }: { children: React.ReactNode }) => {
  return <div className="flex flex-row space-x-0.5">{children}</div>;
};

const ControlButton = React.forwardRef<
  HTMLButtonElement,
  React.ButtonHTMLAttributes<HTMLButtonElement>
>((props, ref) => {
  return (
    <Button
      // @ts-ignore
      ref={ref}
      {...props}
      className={cn(
        'bg-openmct-dark-gray hover:bg-openmct-light-gray w-32 py-6',
        props.className,
      )}
    />
  );
});
ControlButton.displayName = 'ControlButton';

const LeftButton = React.forwardRef<
  HTMLButtonElement,
  React.ButtonHTMLAttributes<HTMLButtonElement>
>((props, ref) => {
  return (
    <ControlButton
      // @ts-ignore
      ref={ref}
      {...props}
      className={cn(
        'bg-openmct-dark-gray hover:bg-openmct-light-gray rounded-r-none py-6 pr-6 pl-8',
        props.className,
      )}
    />
  );
});
LeftButton.displayName = 'LeftButton';

const RightButton = React.forwardRef<
  HTMLButtonElement,
  React.ButtonHTMLAttributes<HTMLButtonElement>
>((props, ref) => {
  return (
    <ControlButton
      // @ts-ignore
      ref={ref}
      {...props}
      className={cn(
        'bg-openmct-dark-gray hover:bg-openmct-light-gray rounded-l-none py-6 pr-8 pl-6',
        props.className,
      )}
    />
  );
});
RightButton.displayName = 'RightButton';
