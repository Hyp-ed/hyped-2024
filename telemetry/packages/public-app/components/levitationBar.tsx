import { Badge, Card, Flex, MarkerBar, Text, Title } from '@tremor/react';

export default () => (
  <Card className="levitate-card">
    <Title>Levitation Height</Title>
    <div className="levitate">
      <div className="levitation">
        <Card className="w-[280px] pt-[170px]  h-[330px]">
          <div className="levitate-2">
            <Text className="mb-3">Levitation Height</Text>
            <Badge>live</Badge>
          </div>
          <MarkerBar value={45} color="fuchsia" className="mt-4" />
        </Card>
      </div>
    </div>
  </Card>
);
