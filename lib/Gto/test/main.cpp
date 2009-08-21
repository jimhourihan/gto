//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//
#include <Gto/Writer.h>
#include <Gto/Reader.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

float fdata[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int   idata[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void write(const char *filename)
{
    cout << "writing " << filename << endl;
    Gto::Writer writer;
    writer.open(filename);

    writer.beginObject("test", "data", 0);
        writer.beginComponent("component_1");
            writer.property("property_1", Gto::Float, 10);
            writer.property("property_2", Gto::Float, 10);
            writer.property("property_3", Gto::Int, 10);
        writer.endComponent();
    writer.endObject();

    writer.beginObject("test2", "data", 0);
        writer.beginComponent("component_1");
            writer.property("property_1", Gto::Float, 10);
            writer.property("property_2", Gto::Float, 10);
            writer.property("property_3", Gto::Int, 10);
            writer.property("property_4", Gto::Int, 10);
        writer.endComponent();
    writer.endObject();

    writer.beginData();
        writer.propertyData(fdata);
        writer.propertyData(fdata);
        writer.propertyData(idata);

        writer.propertyData(fdata);
        writer.propertyData(fdata);
        writer.propertyData(idata);
        writer.propertyData(idata);
    writer.endData();
}

void read(const char *filename)
{
    cout << "reading " << filename << endl;
    Gto::Reader reader;
    reader.open(filename);
}

int main(int, char**)
{
    struct stat s;
    write("test.gto");
    read("test.gto");
    unlink("test.gto");

    if (stat("big_endian.gto",&s) != -1) read("big_endian.gto");
    if (stat("little_endian.gto",&s) != -1) read("little_endian.gto");
}
