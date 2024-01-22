import { PodState } from '@/components/shared/pod-state';

export const PodStateUpdater = ({ podId }: { podId: string }) => {
  return (
    <div className="flex gap-2">
      <PodState podId={podId} />
      <div>
        <p>Change</p>
      </div>
    </div>
  );
};
