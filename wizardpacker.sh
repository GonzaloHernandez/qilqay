#!/bin/bash
rm qilqay-wizard.tar.gz
rm qilqay-wizard.rar
rm qilqay-wizard.zip

mkdir qilqay-wizard
cp qilqay-wizard-src/* qilqay-wizard/
cp qilqay.h qilqaylibrary.h qilqay-wizard/
sed -i 's/\\/\\\\/g' qilqay-wizard/qilqay.h
tar cvzf qilqay-wizard.tar.gz qilqay-wizard
rar a qilqay-wizard.rar qilqay-wizard
zip -r qilqay-wizard.zip qilqay-wizard

rm qilqay-wizard -r
