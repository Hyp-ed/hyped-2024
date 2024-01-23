import { useEffect } from 'react';

type Key = string;
type Combo = 'ctrlKey' | 'shiftKey' | 'altKey' | 'metaKey';
type KeyCombination = [Combo, Key];

/**
 * Adds event listeners for the given key(s) and calls the callback when they are pressed.
 * Also supports key combinations.
 * @param keys The keys to listen for.
 */
export const useKeyPress = (
  keys: (Key | KeyCombination)[],
  callback: (event: KeyboardEvent) => void,
) => {
  const downHandler = (event: KeyboardEvent) => {
    if (keys.includes(event.key)) {
      callback(event);
      return;
    }
    const KeyCombinations = keys.filter(isKeyCombination);
    for (const keyCombination of KeyCombinations) {
      const [combinationKey, key] = keyCombination;
      if (event[combinationKey] && event.key === key) {
        callback(event);
        return;
      }
    }
  };

  useEffect(() => {
    window.addEventListener('keydown', downHandler);
    return () => {
      window.removeEventListener('keydown', downHandler);
    };
  }, []);
};

const isKeyCombination = (key: Key | KeyCombination): key is KeyCombination =>
  Array.isArray(key);
