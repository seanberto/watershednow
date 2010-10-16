Created by Sean Larkin and the team at ThinkShout.com.

## Introduction 
This module contains a number of hacky overrides and small helper functions to
make Drupal's UI not suck. That said, there are some choices here that other
developers might disagree with, such as hiding the Drupal core themes on the
theme settings screen. But hey, we want things to look good. And our code is
well documented. ;)

This module also contains some custom admin screens meant to help consolidate a 
site admin's job of managing a complex Drupal installation.


## Follow Links
We overrided the follow links default theming to add target="_blank".
We also added rel="external me", this tells semantic web users that your profile
links are a part of your web identity. See http://microformats.org/wiki/rel-me

You can retheme the links in your theme using THEME_wn_helper_follow_link($link) 