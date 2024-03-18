import { Linkedin, Facebook, Instagram, Github, Twitter } from 'lucide-react';
import Link from 'next/link';

const SOCIAL_ICONS = {
  Facebook: {
    link: 'https://www.facebook.com/hypedinburgh/',
    component: (
      <Facebook
        color="#c91c10"
        className=" dark:text-white hover:scale-110 transition"
      />
    ),
  },
  github: {
    link: 'https://github.com/hyp-ed/hyped-2024',
    component: (
      <Github
        color="#c91c10"
        className="dark:text-white hover:scale-110 transition"
      />
    ),
  },
  instagram: {
    link: 'https://www.instagram.com/hypedinburgh/',
    component: (
      <Instagram
        color="#c91c10"
        className="dark:text-white hover:scale-110 transition"
      />
    ),
  },
  linkedIn: {
    link: 'https://www.linkedin.com/company/hyp-ed/',
    component: (
      <Linkedin
        color="#c91c10"
        className="dark:text-white hover:scale-110 transition"
      />
    ),
  },
  twitter: {
    link: 'https://twitter.com/hyped_hyperloop',
    component: (
      <Twitter
        color="#c91c10"
        className="dark:text-white hover:scale-110 transition"
      />
    ),
  },
};

export const SocialIcons = () => {
  return (
    <div className="flex flex-row pt-3 gap-4">
      {Object.values(SOCIAL_ICONS).map((urlIcon, index) => (
        <Link href={urlIcon.link} key={index}>
          {urlIcon.component}
        </Link>
      ))}
    </div>
  );
};
