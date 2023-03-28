# Luma3DS FR 3GX Loader
*(N)3DS "Custom Firmware" Traduit en français*

![](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/blob/images/Luma3DS-FR-300px.png)

**ATTENTION : CE PROJET EST ACTUELLEMENT EN COURS DE TRADUCTION**

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
    3. [firmtool](https://github.com/TuxSH/firmtool)
    4. devkitARM+libctru à jour
1. Clonez le dépôt avec `git clone https://github.com/LumaTeam/Luma3DS.git`
2. Exécutez `make`.

    Le `boot.firm` produit est destiné à être copié à la racine de votre carte SD pour une utilisation avec Boot9Strap.

#
### Installation / Utilisation / Fonctionnalités
Consultez le [wiki de Luma3DS FR](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/wiki)

#
### Remerciements
Regardez [ici](https://github.com/BlackFalcon1961/Luma3DS-FR-3GX-Loader/wiki/Remerciements)

#
### Licenses
Ce logiciel est sous licence selon les termes de la GPLv3.  Vous pouvez trouver une copie de la licence dans le fichier `LICENSE.txt`.

Les fichiers du stub GDB sont à la place sous licence triple en tant que MIT ou "GPLv2 ou toute version ultérieure", auquel cas il est spécifié dans l'en-tête du fichier.
