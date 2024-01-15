import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import { cn } from '@/lib/utils';
import {
  ArrowDownToLine,
  ArrowUpFromLine,
  ChevronsDown,
  ChevronsUp,
  Italic,
  MoveDown,
  MoveUp,
  PlugZap,
  Rocket,
  Settings2,
  Siren,
  Unplug,
} from 'lucide-react';
import React from 'react';
import { CONTROLS, control } from '@/lib/controls';
import { SetLevitationHeight } from '@/components/shared/set-levitation-height';
import { Button } from '@tremor/react';

export const FullControls = ({ podId }: { podId: string }) => {
  return (
    <Card className="w-full border-none">
      <CardHeader>
        <CardTitle className="flex gap-2">
          <Settings2 /> Controls
        </CardTitle>
        <CardDescription>Granular pod controls</CardDescription>
      </CardHeader>
      <CardContent>
        <div className="flex gap-4 flex-wrap">
          <div className="space-y-2">
            <ButtonLabel>Start/stop</ButtonLabel>
            <ButtonPair>
              <LeftButton onClick={() => control(podId, CONTROLS.START)}>
                Start <Rocket size={16} />
              </LeftButton>
              <RightButton onClick={() => control(podId, CONTROLS.STOP)}>
                Stop <Siren size={16} />
              </RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Active suspension</ButtonLabel>
            <ButtonPair>
              <LeftButton onClick={() => control(podId, CONTROLS.RAISE)}>
                Raise <ChevronsUp size={16} />
              </LeftButton>
              <RightButton onClick={() => control(podId, CONTROLS.LOWER)}>
                Lower <ChevronsDown size={16} />
              </RightButton>
            </ButtonPair>
            <ControlButton
              className="flex gap-2"
              onClick={() => control(podId, CONTROLS.TILT)}
            >
              Tilt <Italic size={16} />
            </ControlButton>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Friction brakes</ButtonLabel>
            <ButtonPair>
              <LeftButton onClick={() => control(podId, CONTROLS.CLAMP)}>
                Clamp <ArrowDownToLine size={16} />
              </LeftButton>
              <RightButton onClick={() => control(podId, CONTROLS.RETRACT)}>
                Retract <ArrowUpFromLine size={16} />
              </RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>High power</ButtonLabel>
            <ButtonPair>
              <LeftButton onClick={() => control(podId, CONTROLS.START_HP)}>
                Start HP <PlugZap size={16} />
              </LeftButton>
              <RightButton onClick={() => control(podId, CONTROLS.STOP_HP)}>
                Stop HP <Unplug size={16} />
              </RightButton>
            </ButtonPair>
          </div>
          <div className="space-y-2">
            <ButtonLabel>Levitation</ButtonLabel>
            <ButtonPair>
              <LeftButton onClick={() => control(podId, CONTROLS.LEVITATE)}>
                Levitate <MoveUp size={16} />
              </LeftButton>
              <RightButton
                onClick={() => control(podId, CONTROLS.STOP_LEVITATING)}
              >
                Descend <MoveDown size={16} />
              </RightButton>
            </ButtonPair>
            <SetLevitationHeight podId={podId} />
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
        'bg-openmct-dark-gray hover:bg-openmct-light-gray rounded-r-none pr-2 flex gap-2',
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
        'bg-openmct-dark-gray hover:bg-openmct-light-gray rounded-l-none pl-2 flex gap-2',
        props.className,
      )}
    />
  );
});
RightButton.displayName = 'RightButton';
