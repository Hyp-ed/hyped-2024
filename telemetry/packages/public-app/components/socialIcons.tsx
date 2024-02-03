import { Icon } from '@tremor/react';
import React from 'react';

import { Linkedin, Facebook, Instagram, Github, Youtube } from 'lucide-react';

const socialIcons_object = {
  Facebook: (
    <Facebook
      color="#c91c10"
      className=" dark:text-white hover:scale-110 transition"
    />
  ),
  github: (
    <Github
      color="#c91c10"
      className="dark:text-white hover:scale-110 transition"
    />
  ),
  instagram: (
    <Instagram
      color="#c91c10"
      className="dark:text-white hover:scale-110 transition"
    />
  ),
  linkedIn: (
    <Linkedin
      color="#c91c10"
      className="dark:text-white hover:scale-110 transition"
    />
  ),
  youtube: (
    <Youtube
      color="#c91c10"
      className="dark:text-white hover:scale-110 transition"
    />
  ),
};

export const SocialIcons = () => {
  return (
    <div className="flex flex-row pt-3 gap-4">
      {Object.values(socialIcons_object).map((urlIcon, index) => (
        <button key={index}>{urlIcon}</button>
      ))}
    </div>
  );
};
