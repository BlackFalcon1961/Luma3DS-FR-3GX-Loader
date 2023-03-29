# Luma3DS FR 3GX Loader
*(N)3DS "Custom Firmware" Traduit en français*

![](https://user-images.githubusercontent.com/60007836/228280451-ee9f10b8-afa9-4bdb-88b0-5615da23edb1.png)

### Qu'est-ce que c'est ?
**Luma3DS** est un programme pour corriger le logiciel système des consoles portables (New) Nintendo 3DS/2DS "à la volée", ajoutant des fonctionnalités telles que les paramètres de langue par jeu, des capacités de débogage pour les développeurs et la suppression des restrictions appliquées par Nintendo telles que le verrouillage régional.

Il vous permet également d'exécuter du contenu non autorisé ("homebrew") en supprimant les vérifications de signature.
Pour l'utiliser, vous aurez besoin d'une console capable d'exécuter un logiciel homebrew sur le processeur Arm9

Depuis la v8.0, Luma3DS a son propre menu en jeu, activable avec <kbd>L+Bas+Select</kbd> (regardez les [notes de mise à jour](https://github.com/LumaTeam/Luma3DS/releases/tag/v8.0)).

#
### Compilation :
* Prérequis
    1. git
    2. [makerom](https://github.com/jakcron/Project_CTR) dans PATH
    3. [firmtool](https://github.com/BlackFalcon1961/devkitpro-tools/releases/download/tools/firmtool.7z) (déjà compilé)
    4. devkitARM+libctru à jour
1. Clonez le dépôt avec `git clone https://github.com/LumaTeam/Luma3DS.git`
2. Exécutez `make`.

    Le `boot.firm` produit est destiné à être copié à la racine de votre carte SD pour une utilisation avec Boot9Strap.

#
### Installation / Utilisation / Fonctionnalités
Consultez le [wiki de Luma3DS FR](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/wiki)

#
### Remerciements
**[@Nanquitas](https://github.com/Nanquitas)** pour la [version originale](https://github.com/Nanquitas/Luma3DS).  
Regardez [ici](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/wiki/Remerciements) pour accéder à la page des remerciements

#

### Infos supplémentaires :
J'ai également traduit tout le wiki original de [Luma3DS](https://github.com/LumaTeam/Luma3DS/wiki) et ajouté des infos concernant les [plugins 3GX](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/wiki/3GX-Loader).  
J'ai fait de mon mieux pour la traduction, et testé le `boot.firm`.  
Même après avoir tout vérifié, il est possible que certains éléments de texte ne soient pas parfaitement alignés sur les écrans.  
Si effectivement un texte n'est pas correctement aligné il s'agit très certainement d'un oubli de ma part.  

J'ai fait cette traduction afin d'aider certains de mes amis, mais je pense également que ça peut contribuer à la communauté ^^

#

### Licenses
Ce logiciel est sous licence selon les termes de la GPLv3.  Vous pouvez trouver une copie de la licence dans le fichier `LICENSE.txt`.

Les fichiers du stub GDB sont à la place sous licence triple en tant que MIT ou "GPLv2 ou toute version ultérieure", auquel cas il est spécifié dans l'en-tête du fichier.
